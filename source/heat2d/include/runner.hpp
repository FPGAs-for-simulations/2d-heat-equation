#pragma once
// ============================================================
// runner.hpp  –  Top-level entry point
// ============================================================
// Single function that runs both solvers from the same initial
// grid, collects metrics, saves snapshots and metrics CSVs.
// This is the function you would export as the HLS kernel top.
// ============================================================
#include "config.hpp"
#include "heat_types.hpp"

namespace heat2d {

struct RunnerResult {
  Grid serial_grid;
  Grid parallel_grid;
  SolverMetrics serial_metrics;
  SolverMetrics parallel_metrics;
};

// Run both solvers from a shared initial condition.
// cfg controls all parameters including output paths.
// Returns grids and metrics for both solvers.
RunnerResult run_heat2d(const HeatConfig &cfg);

// Convenience: print a compact summary to stdout
void print_summary(const RunnerResult &r);

} // namespace heat2d
