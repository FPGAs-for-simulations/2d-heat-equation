#pragma once
// ============================================================
// hls_types.hpp  –  Data-type definitions for HLS kernel
// ============================================================
// During synthesis (__SYNTHESIS__ is defined by Vitis HLS):
//   hls_dtype  →  ap_fixed<HLS_FP_TOTAL, HLS_FP_INT>
//              OR double  (selectable via HLS_USE_DOUBLE)
//
// During testbench / pure-GCC compilation:
//   hls_dtype  →  double  (always)
//
// All arithmetic in the stencil uses hls_dtype.  The mathematical
// result is identical to double for any fixed-point format with
// sufficient precision because the stencil only adds/subtracts
// values in [bc_min, bc_max] and multiplies by 0.25.
// ============================================================

// ---- Fixed-point precision knobs ----------------------------
// Total width of the fixed-point number (bits)
#define HLS_FP_TOTAL   32
// Number of integer bits (including sign).
// With BCs up to 50.0 and random IC up to 30, we need at least
// ceil(log2(50))+1 = 7 integer bits.  Using 12 gives headroom.
#define HLS_FP_INT     12
// Fractional bits = HLS_FP_TOTAL - HLS_FP_INT = 20  (≈ 1e-6 precision)

// ---- Uncomment to force double even during synthesis --------
#define HLS_USE_DOUBLE

#ifdef __SYNTHESIS__
  #ifndef HLS_USE_DOUBLE
    // Use fixed-point arithmetic on FPGA (lower resource, higher Fmax)
    #include "ap_fixed.h"
    typedef ap_fixed<HLS_FP_TOTAL, HLS_FP_INT>  hls_dtype;
  #else
    // Use IEEE-754 double on FPGA (higher resource, exact match to CPU)
    typedef double hls_dtype;
  #endif
#else
  // ---- Testbench / GCC: always use double -------------------
  // The ap_fixed.h header is not available without Vitis, so we
  // provide a stub typedef.  The #pragma HLS directives below
  // are plain pragmas which GCC silently ignores.
  typedef double hls_dtype;
  // Stub: ap_fixed only used in synthesis path above
#endif

// Integer type for loop bounds / step counts
typedef int   hls_int32;
typedef unsigned int hls_uint32;

// ---- Grid array type alias for clarity ----------------------
// Used as: hls_grid_t u[HLS_GRID_SIZE][HLS_GRID_SIZE];
#include "hls_config.hpp"
typedef hls_dtype hls_grid_row[HLS_GRID_SIZE];

// ---- Per-step metrics (testbench only, not synthesised) -----
struct HlsStepMetrics {
    double compute_ns = 0.0;
    double total_ns   = 0.0;
};

struct HlsSolverMetrics {
    double total_time_ns    = 0.0;
    double total_compute_ns = 0.0;
    int    num_steps        = 0;
    int    num_pe           = 1;
    long long total_flops   = 0;
};
