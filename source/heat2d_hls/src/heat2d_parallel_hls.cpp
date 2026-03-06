// ============================================================
// heat2d_parallel_hls.cpp  –  Parallel HLS kernel implementation
// ============================================================
// Vitis HLS target: xilinx.com:hls:heat2d_parallel_hls
//
// Parallelism model (mapping paper §2 domain-decomp to FPGA)
// ─────────────────────────────────────────────────────────
// Paper (MPI):
//   m processors, each owns rows  [p*(N+2)/m .. (p+1)*(N+2)/m).
//   After each step: exchange ghost rows via Send/Recv.
//   Synchronisation: barrier per step.
//
// This kernel (HLS unroll):
//   HLS_NUM_PE unrolled i-loop bodies each own a strip of rows.
//   HLS_NUM_PE=4 → PE0 computes rows 1,5,9,… ; PE1: 2,6,10,…
//   (cyclic, matching the cyclic array partition of u_a dim=1).
//   No ghost-row communication: all PEs share the same u_a BRAM
//   snapshot.  Rows i-1 and i+1 are in different BRAM banks and
//   can be read simultaneously without any message passing.
//   No barrier: BRAM writes to u_b are naturally conflict-free
//   because each PE writes to different row banks.
//
// Consequence: the computation is mathematically identical to
// the serial kernel (same reads from u_a, same stencil formula,
// same boundary skip logic) but HLS_NUM_PE rows are computed
// per j-pipeline cycle rather than 1.
//
// Architecture
// ────────────
//   TIME_LOOP  (num_steps, variable)
//   └── STENCIL_I  (HLS_N iterations, UNROLL factor=HLS_NUM_PE)
//   └── STENCIL_J  (HLS_N iterations, PIPELINE II=1)
//
// ARRAY_PARTITION dim=1 cyclic factor=HLS_NUM_PE:
//   rows 0, NUM_PE, 2*NUM_PE, … → BRAM bank 0
//   rows 1, NUM_PE+1, …         → BRAM bank 1  …etc.
//   Each PE's stencil reads rows i-1, i, i+1 from at most 3
//   consecutive banks → independent ports → II=1 maintained.
//
// Also partition dim=2 for column accesses (same as serial):
//   j-1, j, j+1 land in different column banks.
//
// With both dimensions partitioned, each PE can read its 5
// stencil operands in one clock cycle → true II=1 per PE.
//
// Latency estimate (synthesis):
//   Σ_{steps} (N/NUM_PE) × (N + fill) ≈ num_steps × N²/NUM_PE
//   = 1/NUM_PE of the serial kernel latency.
// ============================================================

#include "../include/heat2d_parallel_hls.hpp"
#include "../include/hls_stencil.hpp"

#ifndef __SYNTHESIS__
#else
  #include "hls_stream.h"
#endif

void heat2d_parallel_hls(
    hls_dtype  u_in [HLS_GRID_SIZE][HLS_GRID_SIZE],
    hls_dtype  u_out[HLS_GRID_SIZE][HLS_GRID_SIZE],
    hls_int32  num_steps
) {
// ---- Interface (same as serial, drop-in replacement) --------
#pragma HLS INTERFACE ap_memory port=u_in  depth=HLS_GRID_SIZE*HLS_GRID_SIZE
#pragma HLS INTERFACE ap_memory port=u_out depth=HLS_GRID_SIZE*HLS_GRID_SIZE
#pragma HLS INTERFACE s_axilite port=num_steps bundle=ctrl
#pragma HLS INTERFACE ap_ctrl_hs port=return bundle=ctrl

// ---- Internal ping-pong BRAM buffers -------------------------
    hls_dtype u_a[HLS_GRID_SIZE][HLS_GRID_SIZE];
    hls_dtype u_b[HLS_GRID_SIZE][HLS_GRID_SIZE];

// ---- ARRAY_PARTITION: row dimension (domain decomposition) --
// Cyclic partition of dim=1 (rows) by HLS_NUM_PE.
// This creates HLS_NUM_PE independent BRAM banks, one per PE.
// PE p accesses rows p, p+NUM_PE, p+2*NUM_PE, … from bank p.
// The stencil also needs rows i-1 and i+1; with cyclic these
// are in bank (p-1 mod NUM_PE) and (p+1 mod NUM_PE) – separate
// banks, so the parallel reads cost 0 extra cycles.
#pragma HLS ARRAY_PARTITION variable=u_a cyclic factor=HLS_NUM_PE dim=1
#pragma HLS ARRAY_PARTITION variable=u_b cyclic factor=HLS_NUM_PE dim=1

// ---- ARRAY_PARTITION: column dimension (pipeline support) ---
// Same as serial: enables PIPELINE II=1 on inner j-loop by
// placing j-1, j, j+1 in different column banks.
#pragma HLS ARRAY_PARTITION variable=u_a cyclic factor=HLS_COL_PART dim=2
#pragma HLS ARRAY_PARTITION variable=u_b cyclic factor=HLS_COL_PART dim=2

// ---- Load input ---------------------------------------------
    LOAD_I: for (int i = 0; i < HLS_GRID_SIZE; ++i) {
        LOAD_J: for (int j = 0; j < HLS_GRID_SIZE; ++j) {
#pragma HLS PIPELINE II=1
            u_a[i][j] = u_in[i][j];
            u_b[i][j] = u_in[i][j];
        }
    }

// ---- Time integration loop ----------------------------------
    TIME_LOOP: for (hls_int32 k = 0; k < num_steps; ++k) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=HLS_MAX_STEPS_HINT avg=HLS_MAX_STEPS_HINT

// ---- Parallel spatial stencil update ------------------------
// UNROLL factor=HLS_NUM_PE on the row loop:
//   Vitis HLS instantiates HLS_NUM_PE copies of the STENCIL_J
//   body, each accessing its own row bank in u_a/u_b.
//   These NUM_PE copies run truly concurrently in hardware.
//
// Correspondence to paper's domain decomposition:
//   PE 0 ↔ MPI process 0  (rows 1, 1+P, 1+2P, …)
//   PE 1 ↔ MPI process 1  (rows 2, 2+P, …)
//   …
//   "ifirst" and "ilast" logic from the paper are implicit:
//   row 0 and row N+1 are boundary rows – the loop starts at
//   i=1 and ends at i=HLS_N, so boundaries are never updated.
        STENCIL_I: for (int i = 1; i <= HLS_N; ++i) {
// Unroll the row loop by NUM_PE.
// Vitis HLS will create NUM_PE parallel compute units.
#pragma HLS UNROLL factor=HLS_NUM_PE

            STENCIL_J: for (int j = 1; j <= HLS_N; ++j) {
// Pipeline the column loop at II=1.
// With both dimensions partitioned, all 5 stencil reads
// (center, N, S, W, E) resolve to independent BRAM ports
// within the same clock cycle.
#pragma HLS PIPELINE II=1
                u_b[i][j] = apply_stencil(
                    u_a[i  ][j  ],   // center
                    u_a[i-1][j  ],   // north
                    u_a[i+1][j  ],   // south
                    u_a[i  ][j-1],   // west
                    u_a[i  ][j+1]    // east
                );
            } // STENCIL_J
        } // STENCIL_I

// ---- Ping-pong swap (interior only) -------------------------
// Boundaries stay fixed in u_a from the initial LOAD.
        SWAP_I: for (int i = 1; i <= HLS_N; ++i) {
// Also unroll the swap so all row banks update in parallel.
#pragma HLS UNROLL factor=HLS_NUM_PE
            SWAP_J: for (int j = 1; j <= HLS_N; ++j) {
#pragma HLS PIPELINE II=1
                u_a[i][j] = u_b[i][j];
            }
        }

    } // TIME_LOOP

// ---- Write result -------------------------------------------
    STORE_I: for (int i = 0; i < HLS_GRID_SIZE; ++i) {
        STORE_J: for (int j = 0; j < HLS_GRID_SIZE; ++j) {
#pragma HLS PIPELINE II=1
            u_out[i][j] = u_a[i][j];
        }
    }
}
