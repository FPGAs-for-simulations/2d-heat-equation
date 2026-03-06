#pragma once
// ============================================================
// heat_parallel.hpp  –  Domain-decomposition parallel solver
// ============================================================
// Simulates the MPI domain-decomposition algorithm from the paper
// using std::thread.  The (N+2) rows are split among num_threads
// processors.  Each thread owns a contiguous slab of rows and
// keeps ghost rows (top/bottom halos) that are exchanged after
// every compute step – exactly as the paper describes with MPI
// Send/Recv, but realised here as memcpy between shared buffers.
//
// Key variables mirror the paper's pseudocode:
//   ifirst / ilast  –  first/last LOCAL row to update
//   ghost rows      –  local rows 0 and local_rows+1
// ============================================================
#include "config.hpp"
#include "heat_types.hpp"

namespace heat2d {

// Solve from the initial grid in-place (parallel version).
//   in_out  – on entry: initial grid; on exit: final grid
//   metrics – filled with per-step and aggregate statistics
//             metrics.comm_ns counts ghost-row exchange time
void solve_parallel(const HeatConfig& cfg,
                    Grid& in_out,
                    SolverMetrics& metrics);

} // namespace heat2d
