#pragma once
// ============================================================
// heat2d_parallel_hls.hpp  –  Parallel HLS kernel declaration
// ============================================================
#include "hls_types.hpp"

// ============================================================
// heat2d_parallel_hls
// ============================================================
// HLS_NUM_PE-way spatially-parallel explicit FD solver.
// Implements the paper's domain-decomposition concept on FPGA:
//
//   CPU (paper):  m MPI processes each own a strip of rows;
//                 after every step they exchange ghost rows.
//
//   FPGA (here):  HLS_NUM_PE unrolled loop bodies each own a
//                 strip of rows; they ALL read from the same
//                 shared u_old BRAM snapshot so NO explicit
//                 ghost-row communication is needed.  The
//                 stencil reads rows i±1 freely from any bank.
//
// Result: mathematically IDENTICAL to serial (same u_old reads,
// same stencil formula), but with HLS_NUM_PE-fold spatial
// parallelism.  Throughput: N/NUM_PE rows per j-pipeline cycle.
//
// Interface: same as serial kernel (drop-in replacement).
// ============================================================
void heat2d_parallel_hls(
    hls_dtype  u_in [HLS_GRID_SIZE][HLS_GRID_SIZE],
    hls_dtype  u_out[HLS_GRID_SIZE][HLS_GRID_SIZE],
    hls_int32  num_steps
);
