#pragma once
// ============================================================
// heat_serial.hpp  –  Single-processor explicit FD solver
// ============================================================
// Directly maps to the sequential pseudocode in the paper:
//
//   u_new[i][j] = u_old[i][j]
//               + r * (u_old[i+1][j] + u_old[i-1][j]
//                     - 4*u_old[i][j]
//                     + u_old[i][j+1] + u_old[i][j-1])
//
// where r = c * dt / ds^2  (= 0.25 with the paper's dt choice)
// ============================================================
#include "config.hpp"
#include "heat_types.hpp"

namespace heat2d {

// Solve from the initial grid in-place.
//   in_out  – on entry: initial grid (N+2)^2; on exit: final grid
//   metrics – filled with per-step and aggregate statistics
//
// Snapshots are saved according to cfg.snapshot_interval.
void solve_serial(const HeatConfig& cfg,
                  Grid& in_out,
                  SolverMetrics& metrics);

} // namespace heat2d
