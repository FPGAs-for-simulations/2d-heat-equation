#include "config.h"

// Sub-function to perform one grid update step. 
// Using inline(off) ensures only ONE hardware module is built, solving the dangling port issue.
static void compute_step(float *src, float *dst, int size, float factor, 
                         float line_buf[2][MAX_SIZE], float window[3][3]) {
#pragma HLS inline off
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
#pragma HLS PIPELINE II = 1
            
            // Fetch from line buffers - using local variables to stay within 2-port BRAM limit
            float lb0_val = line_buf[0][j];
            float lb1_val = line_buf[1][j];
            float new_val = src[i * size + j];

            // Shift the sliding window
            for (int r = 0; r < 3; r++) {
                window[r][0] = window[r][1];
                window[r][1] = window[r][2];
            }

            // Update window
            window[0][2] = lb0_val;
            window[1][2] = lb1_val;
            window[2][2] = new_val;

            // Update line buffers
            line_buf[0][j] = lb1_val;
            line_buf[1][j] = new_val;

            // Perform stencil calculation for interior points
            if (i >= 2 && j >= 2) {
                int r_i = i - 1;
                int r_j = j - 1;
                if (r_i > 0 && r_i < size - 1 && r_j > 0 && r_j < size - 1) {
                    float center = window[1][1];
                    float res = (window[0][1] + window[2][1] + window[1][0] + window[1][2] - 4.0f * center) * factor + center;
                    dst[r_i * size + r_j] = res;
                }
            }
        }
    }
}

void heat_solver(float *grid_a, float *grid_b, int size, int steps,
                 float factor) {
#pragma HLS INTERFACE m_axi port = grid_a offset = slave bundle = gmem0 depth = 262144
#pragma HLS INTERFACE m_axi port = grid_b offset = slave bundle = gmem1 depth = 262144
#pragma HLS INTERFACE s_axilite port = grid_a
#pragma HLS INTERFACE s_axilite port = grid_b
#pragma HLS INTERFACE s_axilite port = size
#pragma HLS INTERFACE s_axilite port = steps
#pragma HLS INTERFACE s_axilite port = factor
#pragma HLS INTERFACE s_axilite port = return

    static float line_buf[2][MAX_SIZE];
#pragma HLS ARRAY_PARTITION variable = line_buf complete dim = 1
    float window[3][3];
#pragma HLS ARRAY_PARTITION variable = window complete dim = 0

    for (int s = 0; s < steps; s++) {
        // Keeping the explicit branching logic for even/odd steps
        if (s % 2 == 0) {
            compute_step(grid_a, grid_b, size, factor, line_buf, window);
        } else {
            compute_step(grid_b, grid_a, size, factor, line_buf, window);
        }
    }
}
