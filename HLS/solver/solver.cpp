#include "config.h"

void heat_solver(float *grid_a, float *grid_b, int size, int steps,
                 float factor) {
#pragma HLS INTERFACE m_axi port = grid_a offset = slave bundle =              \
    gmem0 depth = 262144
#pragma HLS INTERFACE m_axi port = grid_b offset = slave bundle =              \
    gmem1 depth = 262144
#pragma HLS INTERFACE s_axilite port = grid_a
#pragma HLS INTERFACE s_axilite port = grid_b
#pragma HLS INTERFACE s_axilite port = size
#pragma HLS INTERFACE s_axilite port = steps
#pragma HLS INTERFACE s_axilite port = factor
#pragma HLS INTERFACE s_axilite port = return

  // Local line buffers (BRAM) to store rows for the stencil
  static float line_buf[2][MAX_SIZE];
#pragma HLS ARRAY_PARTITION variable = line_buf complete dim = 1

  // Sliding window (Registers) for the 3x3 stencil
  float window[3][3];
#pragma HLS ARRAY_PARTITION variable = window complete dim = 0

  // Inner loops perform the grid traversal. Outer loop performs time steps.
  for (int s = 0; s < steps; s++) {
    // Explicitly swap pointers based on iteration to ping-pong in DDR
    float *src = (s % 2 == 0) ? grid_a : grid_b;
    float *dst = (s % 2 == 0) ? grid_b : grid_a;

    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
// #pragma HLS PIPELINE II = 1
        // SINGLE READ from global memory per cycle
        float val = src[i * size + j];

        // Shift the sliding window
        for (int r = 0; r < 3; r++) {
          window[r][0] = window[r][1];
          window[r][1] = window[r][2];
        }

        // Update window from BRAM line buffers and the new DDR value
        window[0][2] = line_buf[0][j];
        window[1][2] = line_buf[1][j];
        window[2][2] = val;

        // Update line buffers for the next row
        line_buf[0][j] = line_buf[1][j];
        line_buf[1][j] = val;

        // Perform stencil calculation for interior points
        // i >= 2 and j >= 2 ensures the 3x3 window is fully populated
        if (i >= 2 && j >= 2) {
          int r_i = i - 1;
          int r_j = j - 1;
          // Only update interior points (not boundaries)
          if (r_i > 0 && r_i < size - 1 && r_j > 0 && r_j < size - 1) {
            float center = window[1][1];
            float res = window[0][1] + window[2][1];
            res += window[1][0] + window[1][2];
            float tmp2 = 4.0f * center;
            res -= tmp2;
            res *= factor;
            res += center;
            // SINGLE WRITE to global memory per cycle
            dst[r_i * size + r_j] = res;
          }
        }
      }
    }
  }
}