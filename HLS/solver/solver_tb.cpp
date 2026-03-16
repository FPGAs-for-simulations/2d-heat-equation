#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <iomanip>
#include "config.h"

// Declaration of the kernel (Top function)
void heat_solver(float *grid_a, float *grid_b, int size, int steps, float factor);

int main() {
    int n = DEFAULT_N;
    int size = n + 2;
    int iterations = DEFAULT_ITERATIONS;
    int snapshot = DEFAULT_SNAPSHOT;
    
    // Diffusion factor used by the kernel
    float delta_s = 1.0f / (n + 1);
    float factor = 0.25f;

    std::vector<float> grid_a(size * size);
    std::vector<float> grid_b(size * size);
    
    auto idx = [&](int i, int j) -> int { return i * size + j; };

    /* Initialize grid with boundary conditions and initial values in both buffers */
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            float val;
            if (i == 0) val = DEFAULT_BC_LEFT;
            else if (i == size - 1) val = DEFAULT_BC_RIGHT;
            else if (j == 0) val = DEFAULT_BC_TOP;
            else if (j == size - 1) val = DEFAULT_BC_BOTTOM;
            else val = DEFAULT_INITIAL_VAL;
            
            grid_a[idx(i, j)] = grid_b[idx(i, j)] = val;
        }
    }

    std::ofstream f_hist("heat_history.csv");
    f_hist << "step,x,y,temperature\n";

    std::ofstream f_perf("performance.csv");
    f_perf << "step,time_ms,cumulative_time_ms\n";

    auto start_total = std::chrono::high_resolution_clock::now();

    /* Time stepping in bursts */
    for (int k = 0; k <= iterations; k += snapshot) {
        auto start_step = std::chrono::high_resolution_clock::now();

        /* Log Snapshot from grid_a */
        // After an EVEN number of internal iterations (snapshot=500), 
        // the latest data returns to grid_a.
        std::cout << "Burst starting at Step " << k << " / " << iterations << std::endl;
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                f_hist << k << "," << i * delta_s << "," << j * delta_s << "," << grid_a[idx(i, j)] << "\n";

        if (k == iterations) break;

        /* Call the FPGA Kernel for a burst of iterations */
        // This moves the 500-step loop inside the FPGA
        heat_solver(grid_a.data(), grid_b.data(), size, snapshot, factor);

        auto end_step = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> step_dur = end_step - start_step;
        std::chrono::duration<double, std::milli> total_dur = end_step - start_total;

        f_perf << k + snapshot << "," << step_dur.count() << "," << total_dur.count() << "\n";
    }

    f_hist.close();
    f_perf.close();
    
    auto end_total = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> total_dur_sec = end_total - start_total;

    std::cout << "HLS Burst Simulation complete. Total time: " << std::fixed << std::setprecision(3) 
              << total_dur_sec.count() << " seconds." << std::endl;
    
    return 0;
}
