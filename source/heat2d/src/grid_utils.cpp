// ============================================================
// grid_utils.cpp
// ============================================================
#include "../include/grid_utils.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <random>
#include <stdexcept>
#include <sys/stat.h>

namespace heat2d {

// ------------------------------------------------------------
Grid make_grid(const HeatConfig &cfg) {
  const int G = cfg.grid_size(); // N+2
  Grid g(G, G, 0.0);

  // --- interior initial condition --------------------------
  if (cfg.use_random_ic) {
    std::mt19937 rng(static_cast<unsigned>(cfg.random_seed));
    std::uniform_real_distribution<double> dist(cfg.random_min, cfg.random_max);
    for (int i = 1; i <= cfg.N; ++i)
      for (int j = 1; j <= cfg.N; ++j)
        g.at(i, j) = dist(rng);
  } else {
    for (int i = 1; i <= cfg.N; ++i)
      for (int j = 1; j <= cfg.N; ++j)
        g.at(i, j) = cfg.ic_value;
  }

  // --- boundaries ------------------------------------------
  // left  edge:  i=0,    j=0..N+1
  // right edge:  i=N+1,  j=0..N+1
  for (int j = 0; j < G; ++j) {
    g.at(0, j) = cfg.bc_left;
    g.at(G - 1, j) = cfg.bc_right;
  }
  // bottom edge: j=0,    i=0..N+1
  // top    edge: j=N+1,  i=0..N+1
  for (int i = 0; i < G; ++i) {
    g.at(i, 0) = cfg.bc_bottom;
    g.at(i, G - 1) = cfg.bc_top;
  }
  // Corner assignment order matters: corners get overwritten by
  // the second loop – any consistent choice is fine for constant BCs.
  return g;
}

// ------------------------------------------------------------
Grid copy_grid(const Grid &src) {
  Grid dst(src.rows, src.cols, 0.0);
  dst.data = src.data;
  return dst;
}

// ------------------------------------------------------------
double max_diff(const Grid &a, const Grid &b) {
  assert(a.rows == b.rows && a.cols == b.cols);
  double md = 0.0;
  for (size_t k = 0; k < a.data.size(); ++k)
    md = std::max(md, std::abs(a.data[k] - b.data[k]));
  return md;
}

// ------------------------------------------------------------
bool check_boundaries(const Grid &g, const HeatConfig &cfg, double tol) {
  const int G = cfg.grid_size();
  // Skip corners (j=0, j=G-1) as they are shared by two edges and may hold
  // either boundary value depending on init order.
  for (int j = 1; j < G - 1; ++j) {
    if (std::abs(g.at(0, j) - cfg.bc_left) > tol)
      return false;
    if (std::abs(g.at(G - 1, j) - cfg.bc_right) > tol)
      return false;
  }
  // Skip corners (i=0, i=G-1)
  for (int i = 1; i < G - 1; ++i) {
    if (std::abs(g.at(i, 0) - cfg.bc_bottom) > tol)
      return false;
    if (std::abs(g.at(i, G - 1) - cfg.bc_top) > tol)
      return false;
  }
  return true;
}

// ------------------------------------------------------------
void ensure_dir(const std::string &path) {
  // Simple recursive mkdir for Linux
  std::string p = path;
  // remove trailing slash
  while (!p.empty() && p.back() == '/')
    p.pop_back();
  if (p.empty())
    return;

  struct stat st{};
  if (stat(p.c_str(), &st) == 0)
    return; // already exists

  // create parent first
  auto pos = p.rfind('/');
  if (pos != std::string::npos)
    ensure_dir(p.substr(0, pos));

  mkdir(p.c_str(), 0755);
}

// ------------------------------------------------------------
void save_grid_csv(const Grid &g, const std::string &filepath, int step,
                   double sim_time) {
  std::ofstream f(filepath);
  if (!f.is_open())
    throw std::runtime_error("Cannot open " + filepath);

  f << std::fixed << std::setprecision(10);
  f << "# rows=" << g.rows << " cols=" << g.cols << " step=" << step
    << " time=" << sim_time << "\n";

  for (int i = 0; i < g.rows; ++i) {
    for (int j = 0; j < g.cols; ++j) {
      if (j > 0)
        f << ',';
      f << g.at(i, j);
    }
    f << '\n';
  }
}

// ------------------------------------------------------------
void save_snapshot(const Grid &g, const std::string &output_dir,
                   const std::string &prefix, int step, double sim_time) {
  std::string dir = output_dir + "/" + prefix;
  ensure_dir(dir);

  char buf[32];
  std::snprintf(buf, sizeof(buf), "%07d", step);
  std::string path = dir + "/step_" + buf + ".csv";
  save_grid_csv(g, path, step, sim_time);
}

// ------------------------------------------------------------
void save_metrics_csv(const SolverMetrics &m, const std::string &filepath) {
  std::ofstream f(filepath);
  if (!f.is_open())
    throw std::runtime_error("Cannot open " + filepath);

  f << std::fixed << std::setprecision(4);
  f << "# n=" << m.n << " num_steps=" << m.num_steps
    << " num_threads=" << m.num_threads << " total_time_ns=" << m.total_time_ns
    << " total_compute_ns=" << m.total_compute_ns
    << " total_comm_ns=" << m.total_comm_ns << " total_flops=" << m.total_flops
    << "\n";
  f << "step,compute_ns,comm_ns,total_ns\n";

  for (int k = 0; k < static_cast<int>(m.per_step.size()); ++k) {
    const auto &s = m.per_step[k];
    f << k << ',' << s.compute_ns << ',' << s.comm_ns << ',' << s.total_ns
      << '\n';
  }
}

} // namespace heat2d
