// ============================================================
// heat2d_serial_hls.cpp  –  Serial HLS kernel implementation
// ============================================================
// Vitis HLS target: xilinx.com:hls:heat2d_serial_hls
//
// Architecture
// ────────────
// Two on-chip BRAM ping-pong buffers (u_a, u_b) alternate as
// "current" and "next" each time step, avoiding any copy of the
// full grid.  A pointer-swap variable (ping) selects which
// buffer plays which role each step.
//
//   TIME_LOOP  (num_steps iterations, variable-bound)
//   └── STENCIL_I  (HLS_N iterations, not pipelined –
//   │               flattened with STENCIL_J by Vitis HLS)
//   └── STENCIL_J  (HLS_N iterations, PIPELINE II=1)
//                   reads: center, N, S, W, E from u_a
//                   writes: u_b[i][j]
//
// Pipeline depth of STENCIL_J: II=1 means one new result per
// clock cycle once the pipeline is full.  The tool resolves the
// RAW hazard on u_a (j-1 and j+1 accesses) via the cyclic column
// partition: columns 0,4,8,… in bank 0; 1,5,9,… in bank 1; etc.
// Each cycle needs columns {j-1, j, j+1} which land in at most
// 3 banks → no conflict with factor HLS_COL_PART ≥ 3.
//
// Boundary rows/cols (i=0, i=N+1, j=0, j=N+1) are loaded once
// from u_in and never overwritten, preserving paper §1 BCs.
//
// Latency estimate (synthesis):
//   Σ_{steps} N × (N + pipeline_fill) ≈ num_steps × N² cycles
// ============================================================

#include "../include/heat2d_serial_hls.hpp"
#include "../include/hls_stencil.hpp"

// ---- Vitis HLS headers (no-ops under GCC) -------------------
#ifndef __SYNTHESIS__
  // GCC: #pragma HLS directives are silently ignored.
  // hls_stream.h / ap_fixed.h are NOT included here; they are
  // only needed inside synthesis-guarded blocks.
#else
  #include "hls_stream.h"
#endif

void heat2d_serial_hls(
    hls_dtype  u_in [HLS_GRID_SIZE][HLS_GRID_SIZE],
    hls_dtype  u_out[HLS_GRID_SIZE][HLS_GRID_SIZE],
    hls_int32  num_steps
) {
// ---- Interface directives -----------------------------------
// These tell Vitis HLS how to generate the RTL ports.
// In testbench/csim mode they are ignored by GCC.
#pragma HLS INTERFACE ap_memory port=u_in  depth=HLS_GRID_SIZE*HLS_GRID_SIZE
#pragma HLS INTERFACE ap_memory port=u_out depth=HLS_GRID_SIZE*HLS_GRID_SIZE
#pragma HLS INTERFACE s_axilite port=num_steps bundle=ctrl
#pragma HLS INTERFACE ap_ctrl_hs port=return bundle=ctrl

// ---- Internal ping-pong buffers (BRAM) ----------------------
// u_a: current state (read-only inside STENCIL loops)
// u_b: next state    (write-only inside STENCIL loops)
    hls_dtype u_a[HLS_GRID_SIZE][HLS_GRID_SIZE];
    hls_dtype u_b[HLS_GRID_SIZE][HLS_GRID_SIZE];

// Column-cyclic partition: allows pipelined j-loop to read
// u[i][j-1], u[i][j], u[i][j+1] from different BRAM banks
// in the same clock cycle, achieving II=1 for STENCIL_J.
#pragma HLS ARRAY_PARTITION variable=u_a cyclic factor=HLS_COL_PART dim=2
#pragma HLS ARRAY_PARTITION variable=u_b cyclic factor=HLS_COL_PART dim=2

// ---- Load input into u_a ------------------------------------
// Separate labelled loops allow Vitis HLS to pipeline them.
    LOAD_I: for (int i = 0; i < HLS_GRID_SIZE; ++i) {
        LOAD_J: for (int j = 0; j < HLS_GRID_SIZE; ++j) {
#pragma HLS PIPELINE II=1
            u_a[i][j] = u_in[i][j];
            u_b[i][j] = u_in[i][j];   // pre-fill BCs in u_b too
        }
    }

// ---- Time integration loop ----------------------------------
// Variable loop bound: Vitis HLS uses TRIPCOUNT as a hint for
// latency/throughput estimates in the synthesis report.
    TIME_LOOP: for (hls_int32 k = 0; k < num_steps; ++k) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=HLS_MAX_STEPS_HINT avg=HLS_MAX_STEPS_HINT

// ---- Spatial stencil update (interior points only) ----------
// i-loop is NOT pipelined: each iteration depends on u_a[i-1]
// and u_a[i+1] which are already in u_a from the last step.
// Vitis HLS will flatten STENCIL_I + STENCIL_J into a single
// loop and pipeline STENCIL_J at II=1.
        STENCIL_I: for (int i = 1; i <= HLS_N; ++i) {
            STENCIL_J: for (int j = 1; j <= HLS_N; ++j) {
// Pipeline the innermost loop at II=1.
// Each iteration reads 5 scalars from u_a and writes 1 to u_b.
// With column-cyclic partitioning the 3 column accesses land in
// separate banks → no read-after-read bank conflict.
#pragma HLS PIPELINE II=1
// Label for synthesis report readability:
#pragma HLS LOOP_FLATTEN off   // prevent HLS from flattening only j
                u_b[i][j] = apply_stencil(
                    u_a[i  ][j  ],   // center
                    u_a[i-1][j  ],   // north  (i-1 ≥ 0 since i≥1)
                    u_a[i+1][j  ],   // south  (i+1 ≤ N+1 since i≤N)
                    u_a[i  ][j-1],   // west   (j-1 ≥ 0 since j≥1)
                    u_a[i  ][j+1]    // east   (j+1 ≤ N+1 since j≤N)
                );
            } // STENCIL_J
        } // STENCIL_I

// ---- Ping-pong: swap u_a ↔ u_b by copying ------------------
// Vitis HLS cannot swap BRAM pointers (BRAMs have fixed addresses).
// We copy u_b → u_a for the next iteration.
// Only interior points changed; boundaries remain from LOAD step.
        SWAP_I: for (int i = 1; i <= HLS_N; ++i) {
            SWAP_J: for (int j = 1; j <= HLS_N; ++j) {
#pragma HLS PIPELINE II=1
                u_a[i][j] = u_b[i][j];
            }
        }

    } // TIME_LOOP

// ---- Write result to u_out ----------------------------------
    STORE_I: for (int i = 0; i < HLS_GRID_SIZE; ++i) {
        STORE_J: for (int j = 0; j < HLS_GRID_SIZE; ++j) {
#pragma HLS PIPELINE II=1
            u_out[i][j] = u_a[i][j];
        }
    }
}
