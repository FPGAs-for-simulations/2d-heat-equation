// ============================================================
// heat_parallel.cpp  –  Domain-decomposition parallel solver
// ============================================================
// Simulates the multi-processor algorithm from the paper using
// std::thread.  The (N+2) rows are divided among num_threads
// "processors".  Each thread owns a local sub-grid (slab) and
// keeps one ghost row above and below its slab.
//
// After every compute step the ghost rows are refreshed from
// the neighbours – this is the analogue of MPI Send/Recv shown
// in the paper's pseudocode.
//
// Timing is split identically to the paper:
//   compute_ns = inner stencil loop only
//   comm_ns    = ghost-row exchange (barrier + memcpy)
// ============================================================
#include "../include/heat_parallel.hpp"
#include "../include/grid_utils.hpp"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <condition_variable>
#include <cstring>
#include <mutex>
#include <thread>
#include <vector>

namespace heat2d {

using Clock = std::chrono::high_resolution_clock;

static inline double elapsed_ns(Clock::time_point t0, Clock::time_point t1) {
  return static_cast<double>(
      std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
}

// ============================================================
// Simple reusable barrier (C++17 compatible)
// ============================================================
class Barrier {
  int total_;
  int waiting_ = 0;
  int generation_ = 0;
  std::mutex mtx_;
  std::condition_variable cv_;

public:
  explicit Barrier(int n) : total_(n) {}

  void arrive_and_wait() {
    std::unique_lock<std::mutex> lk(mtx_);
    int gen = generation_;
    if (++waiting_ == total_) {
      ++generation_;
      waiting_ = 0;
      cv_.notify_all();
    } else {
      cv_.wait(lk, [this, gen] { return generation_ != gen; });
    }
  }
};

// ============================================================
// Per-thread work descriptor
// ============================================================
struct ThreadWork {
  int tid;  // this thread's id  (0 .. m-1)
  int m;    // total number of threads
  int N;    // interior grid size
  int G;    // N+2
  double r; // stencil coefficient

  // Global row range owned by this thread (global indices)
  int global_row_start; // first global row
  int global_row_end;   // last  global row (inclusive)
  int ifirst;           // first local row to update (1-based inside slab)
  int ilast;            // last  local row to update (1-based inside slab)

  // Local buffers: (local_rows + 2) x G, with ghost rows at 0 and local_rows+1
  int local_rows; // = global_row_end - global_row_start + 1
  std::vector<double> u_old;
  std::vector<double> u_new;

  // Per-step metrics accumulated by this thread (tid==0 records global)
  std::vector<double> step_compute_ns;
  std::vector<double> step_comm_ns;
  std::vector<double> step_total_ns;

  // Shared objects (non-owning pointers)
  std::vector<ThreadWork *> *all_threads;
  Barrier *barrier;

  int lrows() const { return local_rows + 2; } // including ghost rows
  double &old_at(int li, int j) { return u_old[li * G + j]; }
  double &new_at(int li, int j) { return u_new[li * G + j]; }
};

// ============================================================
// Thread function
// ============================================================
static void thread_func(ThreadWork *w, const HeatConfig &cfg, int num_steps) {
  const int G = w->G;
  const int N = w->N;
  const double r = w->r;

  w->step_compute_ns.reserve(num_steps);
  w->step_comm_ns.reserve(num_steps);
  w->step_total_ns.reserve(num_steps);

  for (int k = 0; k < num_steps; ++k) {
    auto step_t0 = Clock::now();

    // ---- stencil computation ----------------------------
    auto comp_t0 = Clock::now();
    for (int li = w->ifirst; li <= w->ilast; ++li) {
      for (int j = 1; j <= N; ++j) {
        w->new_at(li, j) = w->old_at(li, j) +
                           r * (w->old_at(li + 1, j) + w->old_at(li - 1, j) -
                                4.0 * w->old_at(li, j) + w->old_at(li, j + 1) +
                                w->old_at(li, j - 1));
      }
    }
    auto comp_t1 = Clock::now();

    // swap local buffers
    std::swap(w->u_old, w->u_new);

    // ---- barrier: all threads finish compute step -------
    w->barrier->arrive_and_wait();

    // ---- ghost row exchange (paper: Send/Recv) ----------
    auto comm_t0 = Clock::now();

    // Send bottom ghost to thread tid-1; receive from tid-1
    if (w->tid > 0) {
      ThreadWork *below = (*w->all_threads)[w->tid - 1];
      // copy our row ifirst (local) → their ghost row at ilast+1
      int their_ghost = below->ilast + 1;
      std::memcpy(&below->u_old[their_ghost * G], &w->u_old[w->ifirst * G],
                  static_cast<size_t>(G) * sizeof(double));
    }
    // Send top ghost to thread tid+1; receive from tid+1
    if (w->tid < w->m - 1) {
      ThreadWork *above = (*w->all_threads)[w->tid + 1];
      // copy our row ilast (local) → their ghost row at 0
      std::memcpy(&above->u_old[0], &w->u_old[w->ilast * G],
                  static_cast<size_t>(G) * sizeof(double));
    }

    auto comm_t1 = Clock::now();

    // ---- barrier: all ghost rows written ----------------
    w->barrier->arrive_and_wait();

    auto step_t1 = Clock::now();

    w->step_compute_ns.push_back(elapsed_ns(comp_t0, comp_t1));
    w->step_comm_ns.push_back(elapsed_ns(comm_t0, comm_t1));
    w->step_total_ns.push_back(elapsed_ns(step_t0, step_t1));

    // snapshot written by thread 0 only – saved after step k+1
    if (w->tid == 0 && cfg.snapshot_interval > 0 &&
        ((k + 1) % cfg.snapshot_interval == 0)) {
      // Reassemble global grid from all thread local buffers
      const int Gfull = N + 2;
      Grid snap(Gfull, Gfull, 0.0);
      for (auto *tw : *w->all_threads) {
        for (int li = tw->ifirst; li <= tw->ilast; ++li) {
          int gi = tw->global_row_start + (li - 1);
          for (int j = 0; j < Gfull; ++j)
            snap.at(gi, j) = tw->u_old[li * Gfull + j];
        }
        // boundaries
        if (tw->tid == 0) {
          for (int j = 0; j < Gfull; ++j)
            snap.at(0, j) = cfg.bc_left;
        }
        if (tw->tid == tw->m - 1) {
          for (int j = 0; j < Gfull; ++j)
            snap.at(Gfull - 1, j) = cfg.bc_right;
        }
      }
      for (int i = 0; i < Gfull; ++i) {
        snap.at(i, 0) = cfg.bc_bottom;
        snap.at(i, Gfull - 1) = cfg.bc_top;
      }
      double sim_time = static_cast<double>(k + 1) * cfg.dt();
      save_snapshot(snap, cfg.output_dir, "parallel", k + 1, sim_time);
    }
    w->barrier->arrive_and_wait(); // wait for snapshot write
  }
}

// ============================================================
void solve_parallel(const HeatConfig &cfg, Grid &in_out,
                    SolverMetrics &metrics) {
  const int N = cfg.N;
  const int G = cfg.grid_size();
  const int m = std::max(1, std::min(cfg.num_threads, G));
  const double r = cfg.r();
  const long long flops_per_step = 7LL * N * N;

  metrics.n = N;
  metrics.num_steps = cfg.num_steps;
  metrics.num_threads = m;
  metrics.per_step.reserve(cfg.num_steps);

  // ---- Partition rows among threads -----------------------
  // Paper: processor p owns rows  p*(G/m) .. (p+1)*(G/m)-1
  // We distribute G rows as evenly as possible.
  std::vector<int> row_start(m), row_end(m);
  {
    int base = G / m;
    int extra = G % m;
    int cur = 0;
    for (int p = 0; p < m; ++p) {
      row_start[p] = cur;
      row_end[p] = cur + base - 1 + (p < extra ? 1 : 0);
      cur = row_end[p] + 1;
    }
  }

  // ---- Build per-thread work descriptors ------------------
  Barrier barrier(m);
  std::vector<ThreadWork> workers(m);
  std::vector<ThreadWork *> worker_ptrs(m);
  for (int p = 0; p < m; ++p)
    worker_ptrs[p] = &workers[p];

  for (int p = 0; p < m; ++p) {
    auto &w = workers[p];
    w.tid = p;
    w.m = m;
    w.N = N;
    w.G = G;
    w.r = r;
    w.global_row_start = row_start[p];
    w.global_row_end = row_end[p];
    w.local_rows = row_end[p] - row_start[p] + 1;
    w.all_threads = &worker_ptrs;
    w.barrier = &barrier;

    // local buffer size: (local_rows + 2) * G  (ghost rows top & bottom)
    int lrows = w.lrows();
    w.u_old.assign(static_cast<size_t>(lrows * G), 0.0);
    w.u_new.assign(static_cast<size_t>(lrows * G), 0.0);

    // Copy initial grid into local buffer (local index li = global - row_start
    // + 1)
    for (int li = 1; li <= w.local_rows; ++li) {
      int gi = w.global_row_start + (li - 1);
      for (int j = 0; j < G; ++j) {
        w.u_old[li * G + j] = in_out.at(gi, j);
        w.u_new[li * G + j] = in_out.at(gi, j);
      }
    }

    // Ghost row initialisation from global grid
    // bottom ghost (li=0): row below global_row_start
    if (p > 0) {
      int gi = w.global_row_start - 1;
      for (int j = 0; j < G; ++j)
        w.u_old[0 * G + j] = in_out.at(gi, j);
    }
    // top ghost (li = local_rows+1): row above global_row_end
    if (p < m - 1) {
      int gi = w.global_row_end + 1;
      for (int j = 0; j < G; ++j)
        w.u_old[(w.local_rows + 1) * G + j] = in_out.at(gi, j);
    }

    // ifirst/ilast in LOCAL indices (1-based inside slab)
    w.ifirst = 1;
    w.ilast = w.local_rows;

    // First processor: skip global boundary row 0
    if (p == 0)
      w.ifirst++;
    // Last processor:  skip global boundary row N+1
    if (p == m - 1)
      w.ilast--;

    // Also: rows that are actually interior global indices 1..N
    // Clamp in case the slab is entirely boundary
    int gi_first = w.global_row_start + (w.ifirst - 1);
    int gi_last = w.global_row_start + (w.ilast - 1);
    if (gi_first < 1)
      w.ifirst += (1 - gi_first);
    if (gi_last > N)
      w.ilast -= (gi_last - N);

    // Copy fixed boundary rows to u_new as well
    for (int j = 0; j < G; ++j) {
      w.new_at(1, j) = w.old_at(1, j); // will be overwritten if ifirst>1
    }
  }

  // ---- Save initial state (step 0) before launching threads --
  if (cfg.snapshot_interval > 0) {
    save_snapshot(in_out, cfg.output_dir, "parallel", 0, 0.0);
  }

  // ---- Launch threads -------------------------------------
  auto run_t0 = Clock::now();

  std::vector<std::thread> threads;
  threads.reserve(m);
  for (int p = 0; p < m; ++p)
    threads.emplace_back(thread_func, &workers[p], std::cref(cfg),
                         cfg.num_steps);

  for (auto &t : threads)
    t.join();

  auto run_t1 = Clock::now();
  metrics.total_time_ns = elapsed_ns(run_t0, run_t1);

  // ---- Collect per-step metrics (sum across threads, max for total) ---
  for (int k = 0; k < cfg.num_steps; ++k) {
    double max_compute = 0, sum_comm = 0, max_total = 0;
    for (int p = 0; p < m; ++p) {
      max_compute = std::max(max_compute, workers[p].step_compute_ns[k]);
      sum_comm += workers[p].step_comm_ns[k];
      max_total = std::max(max_total, workers[p].step_total_ns[k]);
    }
    double comm_per_step = sum_comm / static_cast<double>(m);
    metrics.per_step.push_back({max_compute, comm_per_step, max_total});
    metrics.total_compute_ns += max_compute;
    metrics.total_comm_ns += comm_per_step;
    metrics.total_flops += flops_per_step;
  }

  // ---- Reassemble final global grid -----------------------
  for (int p = 0; p < m; ++p) {
    auto &w = workers[p];
    for (int li = 1; li <= w.local_rows; ++li) {
      int gi = w.global_row_start + (li - 1);
      for (int j = 0; j < G; ++j)
        in_out.at(gi, j) = w.u_old[li * G + j];
    }
  }
  // Restore boundary (not touched by solver, but just in case)
  for (int j = 0; j < G; ++j) {
    in_out.at(0, j) = cfg.bc_left;
    in_out.at(G - 1, j) = cfg.bc_right;
  }
  for (int i = 0; i < G; ++i) {
    in_out.at(i, 0) = cfg.bc_bottom;
    in_out.at(i, G - 1) = cfg.bc_top;
  }

  // Save final snapshot only if not already saved by interval
  if (cfg.snapshot_interval > 0 &&
      (cfg.num_steps % cfg.snapshot_interval != 0)) {
    double sim_time = static_cast<double>(cfg.num_steps) * cfg.dt();
    save_snapshot(in_out, cfg.output_dir, "parallel", cfg.num_steps, sim_time);
  }
}

} // namespace heat2d
