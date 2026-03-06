#pragma once
// ============================================================
// hls_stencil.hpp  –  Core stencil and grid helpers (shared)
// ============================================================
// This header is included by BOTH the serial and parallel kernels.
// Everything here is either:
//   a) a small inline function (synthesised inline, zero overhead)
//   b) a helper used only in the testbench (#ifndef __SYNTHESIS__)
//
// Mathematical specification (paper §2, eq.1):
//
//   u_new[i][j] = u_old[i][j]
//               + r * ( u_old[i+1][j]   ← south neighbour
//                     + u_old[i-1][j]   ← north neighbour
//                     - 4*u_old[i][j]   ← centre (×-4)
//                     + u_old[i][j+1]   ← east  neighbour
//                     + u_old[i][j-1] ) ← west  neighbour
//
// where r = HLS_R = 0.25 exactly (c·Δt/Δs² with Δt=Δs²/(4c)).
// Boundary rows/cols (i,j ∈ {0, N+1}) are NEVER updated.
// ============================================================

#include "hls_config.hpp"
#include "hls_types.hpp"

// ============================================================
// apply_stencil  –  the single interior-point update
// ============================================================
// Receives the five neighbouring values directly (not a 2-D array
// pointer) so the HLS tool sees explicit scalar operands and can
// schedule them optimally.  Multiplying by 0.25 is a right-shift
// on fixed-point, and a single FP-multiply on double.
// ============================================================
inline hls_dtype apply_stencil(
        hls_dtype center,
        hls_dtype north,   // u[i-1][j]
        hls_dtype south,   // u[i+1][j]
        hls_dtype west,    // u[i][j-1]
        hls_dtype east     // u[i][j+1]
) {
#pragma HLS INLINE
    return center + static_cast<hls_dtype>(HLS_R)
                  * (north + south + east + west
                     - static_cast<hls_dtype>(4.0) * center);
}

// ============================================================
// Boundary initialisation helpers
// Called once at the start of the testbench to build the
// initial grid; the kernel itself never needs to read BCs
// because boundary rows are copied from u_in and never overwritten.
// ============================================================
#ifndef __SYNTHESIS__
#include <cmath>
#include <random>

// Fill a grid with BCs and interior IC.
// If random_ic==true, interior points are uniform random in [rmin, rmax].
inline void init_grid(hls_dtype g[HLS_GRID_SIZE][HLS_GRID_SIZE],
                      bool  random_ic = false,
                      double rmin = -30.0, double rmax = 30.0,
                      int seed = 42) {
    std::mt19937 rng(static_cast<unsigned>(seed));
    std::uniform_real_distribution<double> dist(rmin, rmax);

    for (int i = 0; i < HLS_GRID_SIZE; ++i) {
        for (int j = 0; j < HLS_GRID_SIZE; ++j) {
            // Default: zero interior
            g[i][j] = static_cast<hls_dtype>(0.0);
        }
    }

    // Interior initial condition
    for (int i = 1; i <= HLS_N; ++i) {
        for (int j = 1; j <= HLS_N; ++j) {
            g[i][j] = static_cast<hls_dtype>(
                random_ic ? dist(rng) : 0.0);
        }
    }

    // Left (i=0) and right (i=N+1) boundary rows
    for (int j = 0; j < HLS_GRID_SIZE; ++j) {
        g[0          ][j] = static_cast<hls_dtype>(HLS_BC_LEFT);
        g[HLS_N + 1  ][j] = static_cast<hls_dtype>(HLS_BC_RIGHT);
    }
    // Bottom (j=0) and top (j=N+1) boundary cols
    for (int i = 0; i < HLS_GRID_SIZE; ++i) {
        g[i][0          ] = static_cast<hls_dtype>(HLS_BC_BOTTOM);
        g[i][HLS_N + 1  ] = static_cast<hls_dtype>(HLS_BC_TOP);
    }
}

// Copy grid a → b
inline void copy_grid(const hls_dtype a[HLS_GRID_SIZE][HLS_GRID_SIZE],
                            hls_dtype b[HLS_GRID_SIZE][HLS_GRID_SIZE]) {
    for (int i = 0; i < HLS_GRID_SIZE; ++i)
        for (int j = 0; j < HLS_GRID_SIZE; ++j)
            b[i][j] = a[i][j];
}

// Max absolute difference between two grids
inline double grid_max_diff(
        const hls_dtype a[HLS_GRID_SIZE][HLS_GRID_SIZE],
        const hls_dtype b[HLS_GRID_SIZE][HLS_GRID_SIZE]) {
    double md = 0.0;
    for (int i = 0; i < HLS_GRID_SIZE; ++i)
        for (int j = 0; j < HLS_GRID_SIZE; ++j) {
            double d = std::abs(static_cast<double>(a[i][j])
                              - static_cast<double>(b[i][j]));
            if (d > md) md = d;
        }
    return md;
}
#endif // !__SYNTHESIS__
