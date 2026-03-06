// ============================================================
// tb_heat2d_hls.cpp  –  Vitis HLS testbench
// ============================================================
// Vitis HLS requirements for a valid testbench:
//   1. Call the top-level function(s) with representative data.
//   2. Compare against a known-good reference (golden model).
//   3. Return 0 on PASS, non-zero on FAIL.
//
// This testbench:
//   A. Runs a golden (pure C++ double) reference model.
//   B. Calls heat2d_serial_hls   – checks for correctness.
//   C. Calls heat2d_parallel_hls – checks for correctness.
//   D. Compares B vs C (serial HLS == parallel HLS).
//   E. Saves CSV snapshots so visualize_hls.py works.
//   F. Reports csim wall-clock timing (not FPGA cycle count –
//      see synthesis report for that).
//
// Snapshot strategy:
//   The HLS kernels don't produce intermediate snapshots (they
//   only output the final state to avoid BRAM port conflicts).
//   The testbench achieves snapshots by calling the kernel
//   in chunks of HLS_SNAPSHOT_INT steps, saving after each chunk.
//   This is equivalent to calling the kernel multiple times
//   from the ARM PS side in a real Zynq/Alveo deployment.
// ============================================================

#include "../include/hls_config.hpp"
#include "../include/hls_types.hpp"
#include "../include/hls_stencil.hpp"
#include "../include/heat2d_serial_hls.hpp"
#include "../include/heat2d_parallel_hls.hpp"
#include "tb_utils.hpp"

#include <chrono>
#include <cstdio>
#include <cstring>
#include <string>

using namespace tb;

// ---- Output directory (matches visualize_hls.py expectation) ----
static const std::string OUTDIR = "output_hls";

// ---- Test state counters ------------------------------------
static int g_pass = 0, g_fail = 0;

static void report(const char* name, bool ok, const char* extra = "") {
    if (ok) { std::printf("  [PASS] %s %s\n", name, extra); ++g_pass; }
    else     { std::printf("  [FAIL] %s %s\n", name, extra); ++g_fail; }
}

// ============================================================
// Helper: run a kernel in snapshot-interval chunks
// ============================================================
// kernel_fn: pointer to heat2d_serial_hls or heat2d_parallel_hls
// out_dir:   subdirectory under OUTDIR for snapshots
// Returns csim wall-clock ns for the compute portion only.
// ============================================================
using KernelFn = void(*)(hls_dtype[HLS_GRID_SIZE][HLS_GRID_SIZE],
                          hls_dtype[HLS_GRID_SIZE][HLS_GRID_SIZE],
                          hls_int32);

static double run_kernel_with_snapshots(
        KernelFn kernel,
        const hls_dtype init[HLS_GRID_SIZE][HLS_GRID_SIZE],
        hls_dtype result[HLS_GRID_SIZE][HLS_GRID_SIZE],
        const std::string& out_dir,
        double& total_ns_out) {

    const int total_steps = HLS_NUM_STEPS;
    const int snap_int    = (HLS_SNAPSHOT_INT > 0) ? HLS_SNAPSHOT_INT : total_steps;

    // Current state buffer
    static hls_dtype cur[HLS_GRID_SIZE][HLS_GRID_SIZE];
    static hls_dtype nxt[HLS_GRID_SIZE][HLS_GRID_SIZE];
    copy_grid(init, cur);

    // Save initial state (step 0)
    ensure_dir(OUTDIR + "/" + out_dir);
    char fname[256];
    std::snprintf(fname, sizeof(fname), "%s/%s/step_%07d.csv",
                  OUTDIR.c_str(), out_dir.c_str(), 0);
    save_grid_csv(cur, fname, 0, 0.0);

    double total_compute_ns = 0.0;
    auto run_t0 = Clock::now();

    int step = 0;
    while (step < total_steps) {
        int chunk = std::min(snap_int, total_steps - step);

        auto t0 = Clock::now();
        kernel(cur, nxt, static_cast<hls_int32>(chunk));
        auto t1 = Clock::now();
        total_compute_ns += elapsed_ns(t0, t1);

        copy_grid(nxt, cur);
        step += chunk;

        // Save snapshot
        double sim_time = static_cast<double>(step) * HLS_DT;
        std::snprintf(fname, sizeof(fname), "%s/%s/step_%07d.csv",
                      OUTDIR.c_str(), out_dir.c_str(), step);
        save_grid_csv(cur, fname, step, sim_time);
    }

    auto run_t1 = Clock::now();
    total_ns_out = elapsed_ns(run_t0, run_t1);
    copy_grid(cur, result);
    return total_compute_ns;
}

// ============================================================
// TEST 1: Constant-solution stability
// If all IC = BC = V, result must equal V everywhere.
// ============================================================
static void test_constant_solution() {
    std::puts("\n-- TEST 1: Constant solution --");

    static hls_dtype g_init[HLS_GRID_SIZE][HLS_GRID_SIZE];
    static hls_dtype g_out [HLS_GRID_SIZE][HLS_GRID_SIZE];

    // Fill everything with 25.0 (matches BCs set in init_grid
    // only if we override after init_grid, so fill directly)
    for (int i = 0; i < HLS_GRID_SIZE; ++i)
        for (int j = 0; j < HLS_GRID_SIZE; ++j)
            g_init[i][j] = static_cast<hls_dtype>(25.0);

    heat2d_serial_hls(g_init, g_out, 100);

    double md = 0.0;
    for (int i = 0; i < HLS_GRID_SIZE; ++i)
        for (int j = 0; j < HLS_GRID_SIZE; ++j) {
            double d = std::abs(static_cast<double>(g_out[i][j]) - 25.0);
            if (d > md) md = d;
        }
    char buf[64]; std::snprintf(buf, sizeof(buf), "max_diff=%.2e", md);
    report("Serial: constant field unchanged (100 steps)", md < 1e-9, buf);

    heat2d_parallel_hls(g_init, g_out, 100);
    md = 0.0;
    for (int i = 0; i < HLS_GRID_SIZE; ++i)
        for (int j = 0; j < HLS_GRID_SIZE; ++j) {
            double d = std::abs(static_cast<double>(g_out[i][j]) - 25.0);
            if (d > md) md = d;
        }
    std::snprintf(buf, sizeof(buf), "max_diff=%.2e", md);
    report("Parallel: constant field unchanged (100 steps)", md < 1e-9, buf);
}

// ============================================================
// TEST 2: Boundary conditions preserved
// ============================================================
static void test_boundary_preserved() {
    std::puts("\n-- TEST 2: Boundary conditions preserved --");

    static hls_dtype g_init[HLS_GRID_SIZE][HLS_GRID_SIZE];
    static hls_dtype g_out [HLS_GRID_SIZE][HLS_GRID_SIZE];
    init_grid(g_init, /*random_ic=*/false);

    heat2d_serial_hls(g_init, g_out, 200);

    bool ok_s = true;
    for (int j = 1; j < HLS_GRID_SIZE-1 && ok_s; ++j) {
        if (std::abs(static_cast<double>(g_out[0        ][j]) - HLS_BC_LEFT  ) > 1e-9) ok_s=false;
        if (std::abs(static_cast<double>(g_out[HLS_N+1  ][j]) - HLS_BC_RIGHT ) > 1e-9) ok_s=false;
    }
    for (int i = 1; i < HLS_GRID_SIZE-1 && ok_s; ++i) {
        if (std::abs(static_cast<double>(g_out[i][0      ]) - HLS_BC_BOTTOM) > 1e-9) ok_s=false;
        if (std::abs(static_cast<double>(g_out[i][HLS_N+1]) - HLS_BC_TOP   ) > 1e-9) ok_s=false;
    }
    report("Serial BC preserved", ok_s);

    heat2d_parallel_hls(g_init, g_out, 200);
    bool ok_p = true;
    for (int j = 1; j < HLS_GRID_SIZE-1 && ok_p; ++j) {
        if (std::abs(static_cast<double>(g_out[0        ][j]) - HLS_BC_LEFT  ) > 1e-9) ok_p=false;
        if (std::abs(static_cast<double>(g_out[HLS_N+1  ][j]) - HLS_BC_RIGHT ) > 1e-9) ok_p=false;
    }
    for (int i = 1; i < HLS_GRID_SIZE-1 && ok_p; ++i) {
        if (std::abs(static_cast<double>(g_out[i][0      ]) - HLS_BC_BOTTOM) > 1e-9) ok_p=false;
        if (std::abs(static_cast<double>(g_out[i][HLS_N+1]) - HLS_BC_TOP   ) > 1e-9) ok_p=false;
    }
    report("Parallel BC preserved", ok_p);
}

// ============================================================
// TEST 3: HLS kernels match golden model (paper formula)
// ============================================================
static void test_match_golden() {
    std::puts("\n-- TEST 3: HLS kernels match golden (paper formula) --");

    static hls_dtype g_init [HLS_GRID_SIZE][HLS_GRID_SIZE];
    static hls_dtype g_ser  [HLS_GRID_SIZE][HLS_GRID_SIZE];
    static hls_dtype g_par  [HLS_GRID_SIZE][HLS_GRID_SIZE];
    static double    g_gold [HLS_GRID_SIZE][HLS_GRID_SIZE];
    init_grid(g_init, /*random_ic=*/true);

    // Run kernels and golden for same steps
    const int steps = 300;
    heat2d_serial_hls  (g_init, g_ser, steps);
    heat2d_parallel_hls(g_init, g_par, steps);

    // Convert init to double for golden
    static double g_init_d[HLS_GRID_SIZE][HLS_GRID_SIZE];
    for (int i = 0; i < HLS_GRID_SIZE; ++i)
        for (int j = 0; j < HLS_GRID_SIZE; ++j)
            g_init_d[i][j] = static_cast<double>(g_init[i][j]);
    golden_run(g_init_d, g_gold, steps);

    double md_sg = max_diff_grids(g_ser, g_gold);
    double md_pg = max_diff_grids(g_par, g_gold);
    double md_sp = max_diff_both_hls(g_ser, g_par);

    char buf[80];
    std::snprintf(buf, sizeof(buf), "max|HLS_serial - golden| = %.2e", md_sg);
    report("Serial   HLS == golden model", md_sg < 1e-9, buf);

    std::snprintf(buf, sizeof(buf), "max|HLS_parallel - golden| = %.2e", md_pg);
    report("Parallel HLS == golden model", md_pg < 1e-9, buf);

    std::snprintf(buf, sizeof(buf), "max|HLS_serial - HLS_parallel| = %.2e", md_sp);
    report("Serial HLS == Parallel HLS (bit-perfect)", md_sp == 0.0, buf);

    // Print a few values for manual inspection
    std::printf("\n  Selected values after %d steps:\n", steps);
    std::printf("  [i,j]  Golden         Serial-HLS     Parallel-HLS\n");
    int pts[4][2] = {{1,1},{1,HLS_N},{HLS_N,1},{HLS_N/2,HLS_N/2}};
    for (auto& p : pts) {
        int i=p[0], j=p[1];
        std::printf("  [%2d,%2d] %14.8f  %14.8f  %14.8f\n",
                    i, j,
                    g_gold[i][j],
                    static_cast<double>(g_ser[i][j]),
                    static_cast<double>(g_par[i][j]));
    }
}

// ============================================================
// TEST 4: Single-step exact stencil check
// Manually compute step 0→1 and compare with HLS kernel.
// ============================================================
static void test_single_step_stencil() {
    std::puts("\n-- TEST 4: Single-step exact stencil (paper eq.1) --");

    static hls_dtype g_init [HLS_GRID_SIZE][HLS_GRID_SIZE];
    static hls_dtype g_ser  [HLS_GRID_SIZE][HLS_GRID_SIZE];
    static hls_dtype g_par  [HLS_GRID_SIZE][HLS_GRID_SIZE];
    init_grid(g_init, false);   // zero IC

    heat2d_serial_hls  (g_init, g_ser, 1);
    heat2d_parallel_hls(g_init, g_par, 1);

    // Manual Python-style check: all interior points are 0,
    // only edge-adjacent points feel BCs.
    // Point [1][1]: neighbours are [0][1]=BC_LEFT=10, [2][1]=0,
    //               [1][0]=BC_BOTTOM=30, [1][2]=0, center=0.
    //   expected = 0 + 0.25*(10 + 0 - 0 + 30 + 0) = 10.0
    double expected_11 = 0.25 * (HLS_BC_LEFT + 0 + HLS_BC_BOTTOM + 0);
    // Point [1][HLS_N]: neighbours [0][N]=10, [2][N]=0,
    //                   [1][N-1]=0, [1][N+1]=BC_TOP=50, center=0
    //   expected = 0 + 0.25*(10 + 0 + 0 + 50) = 15.0
    double expected_1N = 0.25 * (HLS_BC_LEFT + 0 + 0 + HLS_BC_TOP);
    // Center [N/2][N/2]: all interior neighbours = 0
    //   expected = 0
    double expected_cc = 0.0;

    auto chk = [&](const char* lbl, const hls_dtype g[HLS_GRID_SIZE][HLS_GRID_SIZE]) {
        double v11  = static_cast<double>(g[1      ][1      ]);
        double v1N  = static_cast<double>(g[1      ][HLS_N  ]);
        double vcc  = static_cast<double>(g[HLS_N/2][HLS_N/2]);
        bool ok = (std::abs(v11 - expected_11) < 1e-9)
               && (std::abs(v1N - expected_1N) < 1e-9)
               && (std::abs(vcc - expected_cc) < 1e-9);
        char buf[120];
        std::snprintf(buf, sizeof(buf),
                      "[1,1]=%.8f(exp %.8f) [1,N]=%.8f(exp %.8f) [c,c]=%.8f(exp %.8f)",
                      v11, expected_11, v1N, expected_1N, vcc, expected_cc);
        report(lbl, ok, buf);
    };
    chk("Serial   step1 exact stencil", g_ser);
    chk("Parallel step1 exact stencil", g_par);
}

// ============================================================
// TEST 5: Convergence – solution reaches steady state
// At steady state max|u(t) - u(t-N)| ≈ 0.
// Also checks discrete Laplacian ≈ 0 (paper §4).
// ============================================================
static void test_convergence() {
    std::puts("\n-- TEST 5: Convergence to steady state --");

    static hls_dtype g_init[HLS_GRID_SIZE][HLS_GRID_SIZE];
    static hls_dtype g_mid [HLS_GRID_SIZE][HLS_GRID_SIZE];
    static hls_dtype g_end [HLS_GRID_SIZE][HLS_GRID_SIZE];
    init_grid(g_init, true);   // random IC

    const int half = HLS_NUM_STEPS / 2;
    heat2d_serial_hls(g_init, g_mid, half);
    heat2d_serial_hls(g_mid,  g_end, half);

    double max_change = max_diff_both_hls(g_mid, g_end);
    char buf[64];
    std::snprintf(buf, sizeof(buf), "max|u(mid)-u(end)|=%.4e", max_change);
    report("Solution converges (change < 1.0 in second half)", max_change < 1.0, buf);

    // Discrete Laplacian residual at steady state
    double max_lap = 0.0;
    for (int i = 1; i <= HLS_N; ++i) {
        for (int j = 1; j <= HLS_N; ++j) {
            double lap = static_cast<double>(
                g_end[i+1][j] + g_end[i-1][j]
                - 4.0*g_end[i][j]
                + g_end[i][j+1] + g_end[i][j-1]);
            if (std::abs(lap) > max_lap) max_lap = std::abs(lap);
        }
    }
    std::snprintf(buf, sizeof(buf), "max|Lap(u)|=%.4e", max_lap);
    report("Discrete Laplacian → 0 at steady state (< 1e-6)", max_lap < 1e-6, buf);
}

// ============================================================
// TEST 6: Paper example – full 10000-step run, both kernels
// ============================================================
static void test_paper_example() {
    std::puts("\n-- TEST 6: Paper example (N=30, 10000 steps, random IC) --");

    static hls_dtype g_init[HLS_GRID_SIZE][HLS_GRID_SIZE];
    static hls_dtype g_ser [HLS_GRID_SIZE][HLS_GRID_SIZE];
    static hls_dtype g_par [HLS_GRID_SIZE][HLS_GRID_SIZE];
    init_grid(g_init, true);

    double total_ns_s, total_ns_p;
    double compute_ns_s = run_kernel_with_snapshots(
        heat2d_serial_hls,   g_init, g_ser, "serial",   total_ns_s);
    double compute_ns_p = run_kernel_with_snapshots(
        heat2d_parallel_hls, g_init, g_par, "parallel", total_ns_p);

    double md = max_diff_both_hls(g_ser, g_par);
    char buf[80];
    std::snprintf(buf, sizeof(buf), "max|serial-parallel|=%.2e", md);
    report("Serial == Parallel after full run (bit-perfect)", md == 0.0, buf);

    // Save metrics CSVs
    save_metrics_csv(OUTDIR + "/serial_metrics.csv",   "serial",
                     HLS_N, HLS_NUM_STEPS, 1, total_ns_s, compute_ns_s);
    save_metrics_csv(OUTDIR + "/parallel_metrics.csv", "parallel",
                     HLS_N, HLS_NUM_STEPS, HLS_NUM_PE, total_ns_p, compute_ns_p);

    double speedup = (compute_ns_p > 0) ? compute_ns_s / compute_ns_p : 0.0;
    std::printf("\n  === HLS csim timing (N=%d, %d steps) ===\n",
                HLS_N, HLS_NUM_STEPS);
    std::printf("  NOTE: csim runs on CPU (software emulation);\n");
    std::printf("        actual FPGA cycle count comes from csynth report.\n");
    std::printf("  Serial   total=%.3f ms  compute=%.3f ms\n",
                total_ns_s/1e6, compute_ns_s/1e6);
    std::printf("  Parallel total=%.3f ms  compute=%.3f ms  (NUM_PE=%d)\n",
                total_ns_p/1e6, compute_ns_p/1e6, HLS_NUM_PE);
    std::printf("  csim speedup: %.3fx\n", speedup);
    std::printf("  Expected FPGA speedup: ~%dx (synthesis report)\n", HLS_NUM_PE);
}

// ============================================================
// main
// ============================================================
int main() {
    std::puts("============================================================");
    std::printf("  heat2d HLS testbench\n");
    std::printf("  N=%d  STEPS=%d  NUM_PE=%d  r=%.4f\n",
                HLS_N, HLS_NUM_STEPS, HLS_NUM_PE, HLS_R);
    std::printf("  ds=%.8f  dt=%.8e  stability: r<=0.5 %s\n",
                HLS_DS, HLS_DT, HLS_R <= 0.5 ? "OK" : "VIOLATED!");
    std::puts("============================================================");

    ensure_dir(OUTDIR);

    test_constant_solution();
    test_boundary_preserved();
    test_match_golden();
    test_single_step_stencil();
    test_convergence();
    test_paper_example();

    std::puts("\n============================================================");
    std::printf("  Results: %d passed, %d failed\n", g_pass, g_fail);
    std::puts("============================================================\n");

    return (g_fail == 0) ? 0 : 1;
}
