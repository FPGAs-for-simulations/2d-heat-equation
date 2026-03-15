#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <iomanip>
#include "config.h"

/**
 * CSV Output Format (heat_history.csv):
 * step,x,y,temperature
 * 
 * Performance Output Format (performance.csv):
 * step,time_ms,cumulative_time_ms
 */

int main() {
    int n = N_POINTS;
    int size = n + 2;
    
    // Use std::vector for automatic memory management
    std::vector<double> u_old_vec(size * size);
    std::vector<double> u_new_vec(size * size);
    
    auto u_old = [&](int i, int j) -> double& { return u_old_vec[i * size + j]; };
    auto u_new = [&](int i, int j) -> double& { return u_new_vec[i * size + j]; };

    double delta_s = 1.0 / (n + 1);
    double factor = (C_DIFFUSION * ((delta_s * delta_s) / (4.0 * C_DIFFUSION))) / (delta_s * delta_s);

    /* Initialize grid with boundary conditions and initial values */
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == 0) u_old(i, j) = u_new(i, j) = BC_LEFT;
            else if (i == size - 1) u_old(i, j) = u_new(i, j) = BC_RIGHT;
            else if (j == 0) u_old(i, j) = u_new(i, j) = BC_TOP;
            else if (j == size - 1) u_old(i, j) = u_new(i, j) = BC_BOTTOM;
            else u_old(i, j) = INITIAL_VAL;
        }
    }

    std::ofstream f_hist("heat_history.csv");
    f_hist << "step,x,y,temperature\n";

    std::ofstream f_perf("performance.csv");
    f_perf << "step,time_ms,cumulative_time_ms\n";

    auto start_total = std::chrono::high_resolution_clock::now();

    /* Time stepping */
    for (int k = 0; k <= ITERATIONS; k++) {
        auto start_step = std::chrono::high_resolution_clock::now();

        /* Record snapshot sparingly to avoid huge files */
        // Note: For N=3000, 3000*3000 = 9M lines per snapshot. 
        // We'll only do it if k == 0 or k == ITERATIONS to keep it manageable, 
        // or follow original logic if we must. Original was % 500.
        // Let's stick to original but be warned it's huge.
        if (k % 500 == 0) {
            std::cout << "Step " << k << " / " << ITERATIONS << std::endl;
            for (int i = 0; i < size; i++)
                for (int j = 0; j < size; j++)
                    f_hist << k << "," << i * delta_s << "," << j * delta_s << "," << u_old(i, j) << "\n";
        }

        if (k == ITERATIONS) break;

        for (int i = 1; i < size - 1; i++) {
            for (int j = 1; j < size - 1; j++) {
                u_new(i, j) = u_old(i, j) + factor * (
                    u_old(i+1, j) + u_old(i-1, j) + 
                    u_old(i, j+1) + u_old(i, j-1) - 4.0 * u_old(i, j)
                );
            }
        }
        
        // Swap or copy. Copying as per original code.
        for (int i = 1; i < size - 1; i++)
            for (int j = 1; j < size - 1; j++)
                u_old(i, j) = u_new(i, j);

        auto end_step = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> step_dur = end_step - start_step;
        std::chrono::duration<double, std::milli> total_dur = end_step - start_total;

        // Log performance every 10 steps to reduce I/O overhead
        if (k % 10 == 0) {
            f_perf << k << "," << step_dur.count() << "," << total_dur.count() << "\n";
        }
    }

    f_hist.close();
    f_perf.close();
    
    auto end_total = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> total_dur_sec = end_total - start_total;

    std::cout << "Simulation complete. Total time: " << std::fixed << std::setprecision(3) 
              << total_dur_sec.count() << " seconds." << std::endl;
    std::cout << "Data saved to heat_history.csv and performance.csv" << std::endl;
    
    return 0;
}
