#pragma once
// ============================================================
// grid_utils.hpp  –  Grid initialisation, I/O, and comparison
// ============================================================
#include "config.hpp"
#include "heat_types.hpp"
#include <string>

namespace heat2d {

// Build an (N+2)×(N+2) grid from config:
//   – boundaries set to bc_{left,right,bottom,top}
//   – interior set to ic_value (or random if use_random_ic)
Grid make_grid(const HeatConfig& cfg);

// Deep copy
Grid copy_grid(const Grid& src);

// Maximum absolute difference between two grids of equal dimensions
double max_diff(const Grid& a, const Grid& b);

// Returns true if all four boundary rows/cols match config (within tol)
bool check_boundaries(const Grid& g, const HeatConfig& cfg, double tol = 1e-9);

// Create directory (and parents) if it doesn't exist
void ensure_dir(const std::string& path);

// Save grid to a plain CSV file (rows x cols of doubles)
// Header line: "# rows=R cols=C step=S time=T"
void save_grid_csv(const Grid& g, const std::string& filepath,
                   int step = -1, double sim_time = 0.0);

// Convenience: saves to output_dir/prefix/step_NNNNN.csv
void save_snapshot(const Grid& g, const std::string& output_dir,
                   const std::string& prefix, int step, double sim_time);

// Write SolverMetrics per-step data to a CSV file
void save_metrics_csv(const SolverMetrics& m, const std::string& filepath);

} // namespace heat2d
