// ============================================================
// testbench.cpp  –  Correctness and sanity tests for heat2d
// ============================================================
// Vitis HLS testbench style:
//   - returns 0 on ALL tests passed
//   - returns non-zero on any failure
//   - prints PASS / FAIL per test
// ============================================================
#include "config.hpp"
#include "heat_types.hpp"
#include "grid_utils.hpp"
#include "heat_serial.hpp"
#include "heat_parallel.hpp"
#include "runner.hpp"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

using namespace heat2d;

// ============================================================
// Test helpers
// ============================================================
static int g_pass = 0, g_fail = 0;

static void report(const char* name, bool ok, const char* extra = "") {
    if (ok) {
        std::printf("  [PASS] %s %s\n", name, extra);
        ++g_pass;
    } else {
        std::printf("  [FAIL] %s %s\n", name, extra);
        ++g_fail;
    }
}

// ============================================================
// TEST 1: Constant solution
// If all boundaries == IC_VALUE == V, the solution stays V.
// ============================================================
static void test_constant_solution() {
    std::puts("\n-- TEST 1: Constant solution (serial) --");

    constexpr double V = 25.0;
    HeatConfig cfg;
    cfg.N                = 10;
    cfg.num_steps        = 200;
    cfg.c                = 0.1;
    cfg.bc_left = cfg.bc_right = cfg.bc_bottom = cfg.bc_top = V;
    cfg.ic_value         = V;
    cfg.snapshot_interval = 0;
    cfg.save_metrics     = false;
    cfg.output_dir       = "output/test1";

    Grid g = make_grid(cfg);
    SolverMetrics m;
    solve_serial(cfg, g, m);

    double md = 0.0;
    for (double v : g.data) md = std::max(md, std::abs(v - V));
    report("Constant solution stays constant (serial)", md < 1e-10,
           ("max_diff=" + std::to_string(md)).c_str());
}

// ============================================================
// TEST 2: Boundary conditions preserved throughout simulation
// ============================================================
static void test_boundary_preserved() {
    std::puts("\n-- TEST 2: Boundary conditions preserved --");

    HeatConfig cfg;
    cfg.N                = 20;
    cfg.num_steps        = 500;
    cfg.snapshot_interval = 0;
    cfg.save_metrics     = false;
    cfg.output_dir       = "output/test2";

    // Serial
    Grid gs = make_grid(cfg);
    SolverMetrics ms;
    solve_serial(cfg, gs, ms);
    bool ok_s = check_boundaries(gs, cfg);
    report("BC preserved (serial)", ok_s);

    // Parallel
    Grid gp = make_grid(cfg);
    SolverMetrics mp;
    solve_parallel(cfg, gp, mp);
    bool ok_p = check_boundaries(gp, cfg);
    report("BC preserved (parallel)", ok_p);
}

// ============================================================
// TEST 3: Serial and parallel produce identical results
// ============================================================
static void test_serial_parallel_match() {
    std::puts("\n-- TEST 3: Serial == Parallel (bit-level) --");

    HeatConfig cfg;
    cfg.N                = 16;
    cfg.num_steps        = 300;
    cfg.num_threads      = 4;
    cfg.snapshot_interval = 0;
    cfg.save_metrics     = false;
    cfg.output_dir       = "output/test3";

    Grid gs = make_grid(cfg);
    SolverMetrics ms;
    solve_serial(cfg, gs, ms);

    Grid gp = make_grid(cfg);
    SolverMetrics mp;
    solve_parallel(cfg, gp, mp);

    double md = max_diff(gs, gp);
    // Floating point order of operations may differ slightly between
    // sequential and domain-decomp; accept a small tolerance.
    report("Max |serial - parallel| < 1e-10", md < 1e-10,
           ("md=" + std::to_string(md)).c_str());
    report("Max |serial - parallel| < 1e-8 (relaxed)", md < 1e-8,
           ("md=" + std::to_string(md)).c_str());
}

// ============================================================
// TEST 4: Stability – solution must not diverge
// ============================================================
static void test_stability() {
    std::puts("\n-- TEST 4: Stability (no divergence) --");

    HeatConfig cfg;
    cfg.N                = 20;
    cfg.num_steps        = 2000;
    cfg.use_random_ic    = true;
    cfg.random_min       = -100.0;
    cfg.random_max       =  100.0;
    cfg.snapshot_interval = 0;
    cfg.save_metrics     = false;
    cfg.output_dir       = "output/test4";

    (void)cfg; // bc_max/min not used after stability check
    // After many steps the solution should converge to values bounded by BCs
    // At minimum it must not blow up to infinity.
    Grid g = make_grid(cfg);
    SolverMetrics m;
    solve_serial(cfg, g, m);

    bool finite_ok = true;
    for (double v : g.data)
        if (!std::isfinite(v)) { finite_ok = false; break; }
    report("Solution remains finite", finite_ok);
}

// ============================================================
// TEST 5: Metrics sanity
// ============================================================
static void test_metrics_sanity() {
    std::puts("\n-- TEST 5: Metrics sanity --");

    HeatConfig cfg;
    cfg.N                = 10;
    cfg.num_steps        = 100;
    cfg.num_threads      = 2;
    cfg.snapshot_interval = 0;
    cfg.save_metrics     = false;
    cfg.output_dir       = "output/test5";

    Grid gs = make_grid(cfg); SolverMetrics ms; solve_serial  (cfg, gs, ms);
    Grid gp = make_grid(cfg); SolverMetrics mp; solve_parallel(cfg, gp, mp);

    report("Serial   per_step count matches num_steps",
           static_cast<int>(ms.per_step.size()) == cfg.num_steps);
    report("Parallel per_step count matches num_steps",
           static_cast<int>(mp.per_step.size()) == cfg.num_steps);
    report("Serial   total_compute_ns > 0", ms.total_compute_ns > 0.0);
    report("Parallel total_compute_ns > 0", mp.total_compute_ns > 0.0);
    report("Serial   total_flops > 0",      ms.total_flops > 0);
    report("Parallel total_flops > 0",      mp.total_flops > 0);
    report("Serial   num_threads == 1",     ms.num_threads == 1);
    report("Parallel num_threads == 2",     mp.num_threads == 2);

    // Flop count check: 7 flops * N^2 * steps
    long long expected = 7LL * cfg.N * cfg.N * cfg.num_steps;
    report("Serial   flop count correct",
           ms.total_flops == expected,
           ("got=" + std::to_string(ms.total_flops)
            + " expected=" + std::to_string(expected)).c_str());
    report("Parallel flop count correct",
           mp.total_flops == expected,
           ("got=" + std::to_string(mp.total_flops)
            + " expected=" + std::to_string(expected)).c_str());
}

// ============================================================
// TEST 6: Paper example (N=30, 10000 steps) – smoke test
// ============================================================
static void test_paper_example() {
    std::puts("\n-- TEST 6: Paper example (N=30, 2000 steps) --");
    // Full 10000 steps would be slow in a testbench; use 2000 for quick check.

    HeatConfig cfg;
    cfg.N                = 30;
    cfg.num_steps        = 2000;   // shortened for TB speed
    cfg.c                = 0.1;
    cfg.bc_left          = 10.0;
    cfg.bc_right         = 40.0;
    cfg.bc_bottom        = 30.0;
    cfg.bc_top           = 50.0;
    cfg.ic_value         = 0.0;
    cfg.num_threads      = 4;
    cfg.snapshot_interval = 0;
    cfg.save_metrics     = false;
    cfg.output_dir       = "output/test6";

    Grid gs = make_grid(cfg); SolverMetrics ms; solve_serial  (cfg, gs, ms);
    Grid gp = make_grid(cfg); SolverMetrics mp; solve_parallel(cfg, gp, mp);

    double md = max_diff(gs, gp);
    report("Serial   BC preserved",        check_boundaries(gs, cfg));
    report("Parallel BC preserved",        check_boundaries(gp, cfg));
    report("Serial == Parallel (< 1e-8)", md < 1e-8,
           ("md=" + std::to_string(md)).c_str());

    // Print timing
    std::printf("  Serial   total=%.3f ms  avg_step=%.4f ms\n",
                ms.total_time_ns / 1e6, ms.avg_step_ns() / 1e6);
    std::printf("  Parallel total=%.3f ms  avg_step=%.4f ms  threads=%d\n",
                mp.total_time_ns / 1e6, mp.avg_step_ns() / 1e6, mp.num_threads);
    std::printf("  Compute speedup: %.3fx\n",
                ms.total_compute_ns / (mp.total_compute_ns > 0 ? mp.total_compute_ns : 1));
}

// ============================================================
// TEST 7: Thread count sensitivity (1,2,4,8 threads → same result)
// ============================================================
static void test_thread_count() {
    std::puts("\n-- TEST 7: Thread-count independence --");

    HeatConfig base;
    base.N                = 12;
    base.num_steps        = 100;
    base.snapshot_interval = 0;
    base.save_metrics     = false;
    base.output_dir       = "output/test7";

    // Reference: single thread parallel
    Grid ref = make_grid(base);
    { HeatConfig c = base; c.num_threads = 1; SolverMetrics m; solve_parallel(c, ref, m); }

    for (int t : {2, 3, 4, 7}) {
        Grid g = make_grid(base);
        HeatConfig c = base; c.num_threads = t;
        SolverMetrics m;
        solve_parallel(c, g, m);
        double md = max_diff(ref, g);
        char label[64];
        std::snprintf(label, sizeof(label), "threads=%d vs threads=1", t);
        report(label, md < 1e-8,
               ("md=" + std::to_string(md)).c_str());
    }
}

// ============================================================
// main
// ============================================================
int main() {
    std::puts("====================================================");
    std::puts("  heat2d testbench");
    std::puts("====================================================");

    test_constant_solution();
    test_boundary_preserved();
    test_serial_parallel_match();
    test_stability();
    test_metrics_sanity();
    test_paper_example();
    test_thread_count();

    std::puts("\n====================================================");
    std::printf("  Results: %d passed, %d failed\n", g_pass, g_fail);
    std::puts("====================================================\n");

    return (g_fail == 0) ? 0 : 1;
}
