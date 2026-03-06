#pragma once
// ============================================================
// heat2d_serial_hls.hpp  –  Serial HLS kernel declaration
// ============================================================
#include "hls_types.hpp"

// ============================================================
// heat2d_serial_hls
// ============================================================
// Single-PE explicit finite-difference solver.
// Maps directly to the paper's "single processor" pseudocode.
//
// Interface:
//   u_in   [GRID][GRID]  – initial temperature field (read-only)
//   u_out  [GRID][GRID]  – field after num_steps updates
//   num_steps            – how many time steps to compute
//
// HLS interface pragmas (see .cpp):
//   u_in, u_out → ap_memory (BRAM) or m_axi (DDR) per config
//   num_steps   → s_axilite scalar
//   return      → ap_ctrl_hs (done signal)
// ============================================================
void heat2d_serial_hls(
    hls_dtype  u_in [HLS_GRID_SIZE][HLS_GRID_SIZE],
    hls_dtype  u_out[HLS_GRID_SIZE][HLS_GRID_SIZE],
    hls_int32  num_steps
);
