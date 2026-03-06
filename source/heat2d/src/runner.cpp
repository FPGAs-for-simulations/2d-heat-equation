// ============================================================
// runner.cpp  –  Top-level runner (serial + parallel)
// ============================================================
#include "../include/runner.hpp"
#include "../include/grid_utils.hpp"
#include "../include/heat_serial.hpp"
#include "../include/heat_parallel.hpp"

#include <cmath>
#include <cstdio>
#include <iomanip>
#include <iostream>

namespace heat2d {

RunnerResult run_heat2d(const HeatConfig& cfg) {
    ensure_dir(cfg.output_dir);

    RunnerResult res;

    // Build shared initial grid
    Grid init = make_grid(cfg);

    // ---- Serial run -----------------------------------------
    res.serial_grid = copy_grid(init);
    solve_serial(cfg, res.serial_grid, res.serial_metrics);

    if (cfg.save_metrics)
        save_metrics_csv(res.serial_metrics,
                         cfg.output_dir + "/serial_metrics.csv");

    // ---- Parallel run (same initial grid) -------------------
    res.parallel_grid = copy_grid(init);
    solve_parallel(cfg, res.parallel_grid, res.parallel_metrics);

    if (cfg.save_metrics)
        save_metrics_csv(res.parallel_metrics,
                         cfg.output_dir + "/parallel_metrics.csv");

    return res;
}

// ------------------------------------------------------------
void print_summary(const RunnerResult& r) {
    const auto& sm = r.serial_metrics;
    const auto& pm = r.parallel_metrics;

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "\n===== heat2d run summary =====\n";
    std::cout << "  Grid size    : " << (sm.n + 2) << " x " << (sm.n + 2)
              << "  (N=" << sm.n << " interior)\n";
    std::cout << "  Time steps   : " << sm.num_steps << "\n";
    std::cout << "  Threads(par) : " << pm.num_threads << "\n\n";

    auto print_row = [](const char* label, const SolverMetrics& m) {
        double total_ms   = m.total_time_ns    / 1e6;
        double compute_ms = m.total_compute_ns / 1e6;
        double comm_ms    = m.total_comm_ns    / 1e6;
        double avg_step   = m.avg_step_ns()    / 1e6;
        double avg_comp   = m.avg_compute_ns() / 1e6;
        double gflops     = m.gflops();

        std::printf("  %-10s | total=%9.3f ms  compute=%9.3f ms  comm=%9.3f ms\n",
                    label, total_ms, compute_ms, comm_ms);
        std::printf("  %-10s | avg_step=%7.4f ms  avg_compute=%7.4f ms  GFLOPS=%.4f\n",
                    "", avg_step, avg_comp, gflops);
    };

    print_row("Serial", sm);
    print_row("Parallel", pm);

    double speedup = (pm.total_compute_ns > 0)
        ? sm.total_compute_ns / pm.total_compute_ns : 0.0;
    double max_diff_val = max_diff(r.serial_grid, r.parallel_grid);

    std::printf("\n  Compute speedup (serial/parallel): %.3fx\n", speedup);
    std::printf("  Max |serial - parallel| : %.2e\n\n", max_diff_val);
}

} // namespace heat2d
