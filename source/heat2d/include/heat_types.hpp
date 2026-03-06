#pragma once
// ============================================================
// heat_types.hpp  –  Shared data types
// ============================================================
#include <vector>
#include <cstdint>
#include <numeric>
#include <cmath>

namespace heat2d {

// ------------------------------------------------------------
// Grid  –  flat row-major 2-D array
// Access: grid.at(i, j) where i = x-row, j = y-col
// ------------------------------------------------------------
struct Grid {
    int rows = 0;
    int cols = 0;
    std::vector<double> data;

    Grid() = default;
    Grid(int r, int c, double init = 0.0)
        : rows(r), cols(c), data(static_cast<size_t>(r * c), init) {}

    double&       at(int i, int j)       { return data[i * cols + j]; }
    const double& at(int i, int j) const { return data[i * cols + j]; }
    double*       ptr()                  { return data.data(); }
    const double* ptr()            const { return data.data(); }
    int           size()           const { return rows * cols; }
};

// ------------------------------------------------------------
// StepMetrics  –  timing for a single time step (nanoseconds)
// ------------------------------------------------------------
struct StepMetrics {
    double compute_ns = 0.0;  // stencil kernel time
    double comm_ns    = 0.0;  // ghost-row exchange / barrier wait
    double total_ns   = 0.0;  // full step (compute + comm + overhead)
};

// ------------------------------------------------------------
// SolverMetrics  –  aggregated statistics for a complete run
// ------------------------------------------------------------
struct SolverMetrics {
    std::vector<StepMetrics> per_step;   // one entry per step

    double total_time_ns    = 0.0;
    double total_compute_ns = 0.0;
    double total_comm_ns    = 0.0;
    long long total_flops   = 0;

    int n           = 0;   // interior grid size
    int num_steps   = 0;
    int num_threads = 1;   // 1 = serial

    // ---- Convenience accessors --------------------------------
    double avg_step_ns()    const {
        if (per_step.empty()) return 0.0;
        double s = 0;
        for (const auto& m : per_step) s += m.total_ns;
        return s / static_cast<double>(per_step.size());
    }
    double avg_compute_ns() const {
        if (per_step.empty()) return 0.0;
        double s = 0;
        for (const auto& m : per_step) s += m.compute_ns;
        return s / static_cast<double>(per_step.size());
    }
    // GFLOPS based only on computation (excludes comm)
    double gflops() const {
        if (total_compute_ns <= 0.0) return 0.0;
        return static_cast<double>(total_flops) / (total_compute_ns * 1.0); // ns->s: /1e9, GFLOPS: /1e9 → cancel
    }
};

} // namespace heat2d
