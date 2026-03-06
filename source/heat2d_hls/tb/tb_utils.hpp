#pragma once
// ============================================================
// tb_utils.hpp  –  Testbench utilities (golden model + I/O)
// ============================================================
// Never included from kernel source files.
// All contents are testbench-only (#ifndef __SYNTHESIS__ implied
// by the fact that only tb_heat2d_hls.cpp includes this).
// ============================================================
#include "../include/hls_config.hpp"
#include "../include/hls_types.hpp"
#include "../include/hls_stencil.hpp"

#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <string>
#include <sys/stat.h>

namespace tb {

using Clock = std::chrono::high_resolution_clock;
static inline double elapsed_ns(Clock::time_point t0, Clock::time_point t1) {
    return static_cast<double>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(t1-t0).count());
}

// ============================================================
// golden_step – apply one time step using plain C++ doubles.
// This is the unambiguous reference implementation.
// The stencil formula is written out explicitly so the
// correspondence to the paper's equation is obvious:
//
//   u_new[i][j] = u_old[i][j]
//               + 0.25 * ( u_old[i-1][j] + u_old[i+1][j]
//                          - 4*u_old[i][j]
//                          + u_old[i][j-1] + u_old[i][j+1] )
// ============================================================
inline void golden_step(
        const double g_old[HLS_GRID_SIZE][HLS_GRID_SIZE],
              double g_new[HLS_GRID_SIZE][HLS_GRID_SIZE]) {
    for (int i = 1; i <= HLS_N; ++i) {
        for (int j = 1; j <= HLS_N; ++j) {
            g_new[i][j] = g_old[i][j]
                + 0.25 * (  g_old[i-1][j]
                           + g_old[i+1][j]
                           - 4.0 * g_old[i][j]
                           + g_old[i][j-1]
                           + g_old[i][j+1] );
        }
    }
    // Boundaries never change
    for (int j = 0; j < HLS_GRID_SIZE; ++j) {
        g_new[0          ][j] = g_old[0          ][j];
        g_new[HLS_N + 1  ][j] = g_old[HLS_N + 1  ][j];
    }
    for (int i = 1; i <= HLS_N; ++i) {
        g_new[i][0          ] = g_old[i][0          ];
        g_new[i][HLS_N + 1  ] = g_old[i][HLS_N + 1  ];
    }
}

// Run golden model for 'steps' steps; copy-swaps internally.
inline void golden_run(
        const double g_init[HLS_GRID_SIZE][HLS_GRID_SIZE],
              double g_out [HLS_GRID_SIZE][HLS_GRID_SIZE],
        int steps) {
    static double buf_a[HLS_GRID_SIZE][HLS_GRID_SIZE];
    static double buf_b[HLS_GRID_SIZE][HLS_GRID_SIZE];
    for (int i = 0; i < HLS_GRID_SIZE; ++i)
        for (int j = 0; j < HLS_GRID_SIZE; ++j)
            buf_a[i][j] = g_init[i][j];
    for (int k = 0; k < steps; ++k) {
        golden_step(buf_a, buf_b);
        for (int i = 0; i < HLS_GRID_SIZE; ++i)
            for (int j = 0; j < HLS_GRID_SIZE; ++j)
                buf_a[i][j] = buf_b[i][j];
    }
    for (int i = 0; i < HLS_GRID_SIZE; ++i)
        for (int j = 0; j < HLS_GRID_SIZE; ++j)
            g_out[i][j] = buf_a[i][j];
}

// ============================================================
// I/O helpers
// ============================================================
inline void ensure_dir(const std::string& path) {
    struct stat st{};
    if (stat(path.c_str(), &st) == 0) return;
    auto pos = path.rfind('/');
    if (pos != std::string::npos) ensure_dir(path.substr(0, pos));
    mkdir(path.c_str(), 0755);
}

// Save an hls_dtype grid to CSV in the same format as heat2d.
// Testbench uses this so visualize.py works on HLS output without changes.
inline void save_grid_csv(
        const hls_dtype g[HLS_GRID_SIZE][HLS_GRID_SIZE],
        const std::string& path, int step, double sim_time) {
    ensure_dir(path.substr(0, path.rfind('/')));
    std::ofstream f(path);
    if (!f) { std::fprintf(stderr, "Cannot open %s\n", path.c_str()); return; }
    f << std::fixed << std::setprecision(10);
    f << "# rows=" << HLS_GRID_SIZE << " cols=" << HLS_GRID_SIZE
      << " step=" << step << " time=" << sim_time << "\n";
    for (int i = 0; i < HLS_GRID_SIZE; ++i) {
        for (int j = 0; j < HLS_GRID_SIZE; ++j) {
            if (j) f << ',';
            f << static_cast<double>(g[i][j]);
        }
        f << '\n';
    }
}

// Save a double grid (golden model output)
inline void save_golden_csv(
        const double g[HLS_GRID_SIZE][HLS_GRID_SIZE],
        const std::string& path, int step, double sim_time) {
    ensure_dir(path.substr(0, path.rfind('/')));
    std::ofstream f(path);
    if (!f) { std::fprintf(stderr, "Cannot open %s\n", path.c_str()); return; }
    f << std::fixed << std::setprecision(10);
    f << "# rows=" << HLS_GRID_SIZE << " cols=" << HLS_GRID_SIZE
      << " step=" << step << " time=" << sim_time << "\n";
    for (int i = 0; i < HLS_GRID_SIZE; ++i) {
        for (int j = 0; j < HLS_GRID_SIZE; ++j) {
            if (j) f << ',';
            f << g[i][j];
        }
        f << '\n';
    }
}

// Write metrics CSV (same format as heat2d's save_metrics_csv)
inline void save_metrics_csv(
        const std::string& path,
        const char* label,
        int n, int num_steps, int num_pe,
        double total_ns, double compute_ns) {
    ensure_dir(path.substr(0, path.rfind('/')));
    std::ofstream f(path);
    if (!f) return;
    f << std::fixed << std::setprecision(4);
    f << "# n=" << n
      << " num_steps=" << num_steps
      << " num_threads=" << num_pe
      << " total_time_ns=" << total_ns
      << " total_compute_ns=" << compute_ns
      << " total_comm_ns=0"
      << " total_flops=" << 7LL * n * n * num_steps
      << "\n";
    f << "step,compute_ns,comm_ns,total_ns\n";
    // For HLS csim, we only have aggregate timing, not per-step.
    // Distribute evenly across steps for visualiser compatibility.
    double step_compute = compute_ns / num_steps;
    double step_total   = total_ns   / num_steps;
    for (int k = 0; k < num_steps; ++k) {
        f << k << ',' << step_compute << ",0," << step_total << '\n';
    }
    (void)label;
}

// ============================================================
// Grid compare helpers
// ============================================================
template<typename T>
inline double max_diff_grids(
        const T a[HLS_GRID_SIZE][HLS_GRID_SIZE],
        const double b[HLS_GRID_SIZE][HLS_GRID_SIZE]) {
    double md = 0.0;
    for (int i = 0; i < HLS_GRID_SIZE; ++i)
        for (int j = 0; j < HLS_GRID_SIZE; ++j) {
            double d = std::abs(static_cast<double>(a[i][j]) - b[i][j]);
            if (d > md) md = d;
        }
    return md;
}

template<typename T>
inline double max_diff_both_hls(
        const T a[HLS_GRID_SIZE][HLS_GRID_SIZE],
        const T b[HLS_GRID_SIZE][HLS_GRID_SIZE]) {
    double md = 0.0;
    for (int i = 0; i < HLS_GRID_SIZE; ++i)
        for (int j = 0; j < HLS_GRID_SIZE; ++j) {
            double d = std::abs(static_cast<double>(a[i][j])
                              - static_cast<double>(b[i][j]));
            if (d > md) md = d;
        }
    return md;
}

} // namespace tb
