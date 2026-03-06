// ============================================================
// heat_serial.cpp  –  Sequential explicit FD solver
// ============================================================
// Implements the single-processor algorithm from the paper:
//
//   u_new[i][j] = u_old[i][j]
//               + r * ( u_old[i+1][j] + u_old[i-1][j]
//                      - 4*u_old[i][j]
//                      + u_old[i][j+1] + u_old[i][j-1] )
//
// Boundary rows/cols (i,j = 0 or N+1) are never updated.
// ============================================================
#include "../include/heat_serial.hpp"
#include "../include/grid_utils.hpp"

#include <chrono>

namespace heat2d {

using Clock = std::chrono::high_resolution_clock;

static inline double elapsed_ns(Clock::time_point t0, Clock::time_point t1) {
    return static_cast<double>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
}

void solve_serial(const HeatConfig& cfg, Grid& in_out, SolverMetrics& metrics) {
    const int N = cfg.N;
    const int G = cfg.grid_size();   // N+2
    const double r = cfg.r();        // 0.25
    // flops per inner point per step (5-point stencil: 4 add + 1*-4 + mul r + add u_old = 7)
    const long long flops_per_step = 7LL * N * N;

    metrics.n           = N;
    metrics.num_steps   = cfg.num_steps;
    metrics.num_threads = 1;
    metrics.per_step.reserve(cfg.num_steps);

    Grid u_old = copy_grid(in_out);
    Grid u_new(G, G, 0.0);

    // Copy boundary into u_new (stays constant)
    for (int j = 0; j < G; ++j) {
        u_new.at(0,   j) = cfg.bc_left;
        u_new.at(G-1, j) = cfg.bc_right;
    }
    for (int i = 0; i < G; ++i) {
        u_new.at(i, 0  ) = cfg.bc_bottom;
        u_new.at(i, G-1) = cfg.bc_top;
    }

    auto run_start = Clock::now();

    // Save initial state (step 0 = state before any update)
    if (cfg.snapshot_interval > 0) {
        save_snapshot(u_old, cfg.output_dir, "serial", 0, 0.0);
    }

    for (int k = 0; k < cfg.num_steps; ++k) {
        auto step_start = Clock::now();

        // --- stencil computation ----------------------------
        auto compute_start = Clock::now();
        for (int i = 1; i <= N; ++i) {
            for (int j = 1; j <= N; ++j) {
                u_new.at(i, j) =
                    u_old.at(i, j)
                    + r * (  u_old.at(i+1, j)
                           + u_old.at(i-1, j)
                           - 4.0 * u_old.at(i, j)
                           + u_old.at(i, j+1)
                           + u_old.at(i, j-1) );
            }
        }
        auto compute_end = Clock::now();

        // swap buffers (boundary stays as-is)
        std::swap(u_old.data, u_new.data);

        auto step_end = Clock::now();

        double compute_ns = elapsed_ns(compute_start, compute_end);
        double total_ns   = elapsed_ns(step_start,    step_end);

        metrics.per_step.push_back({ compute_ns, 0.0, total_ns });
        metrics.total_compute_ns += compute_ns;
        metrics.total_flops      += flops_per_step;

        // snapshot: save state after step k+1
        if (cfg.snapshot_interval > 0 && ((k + 1) % cfg.snapshot_interval == 0)) {
            double sim_time = static_cast<double>(k + 1) * cfg.dt();
            save_snapshot(u_old, cfg.output_dir, "serial", k + 1, sim_time);
        }
    }

    auto run_end = Clock::now();
    metrics.total_time_ns = elapsed_ns(run_start, run_end);
    metrics.total_comm_ns = 0.0;

    // If num_steps is not a multiple of snapshot_interval, save final state
    if (cfg.snapshot_interval > 0 && (cfg.num_steps % cfg.snapshot_interval != 0)) {
        double sim_time = static_cast<double>(cfg.num_steps) * cfg.dt();
        save_snapshot(u_old, cfg.output_dir, "serial", cfg.num_steps, sim_time);
    }

    in_out = std::move(u_old);
}

} // namespace heat2d
