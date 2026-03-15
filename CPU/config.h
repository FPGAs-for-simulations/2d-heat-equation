#ifndef CONFIG_H
#define CONFIG_H

/* Constants */
#define C_DIFFUSION 0.1    /* Diffusion coefficient 'c' */
#define N_POINTS    300   /* Number of interior points per dimension (total grid is N+2) */
#define ITERATIONS  10000 /* Total time steps to compute */

/* Boundary Conditions (constant temperatures) */
#define BC_LEFT     10.0
#define BC_RIGHT    40.0
#define BC_TOP      30.0
#define BC_BOTTOM   50.0

/* Initial value for interior points */
#define INITIAL_VAL 30.0

/* Derived constants (Calculated in solver, but listed here for clarity)
 * Delta_s = 1.0 / (N_POINTS + 1)
 * Delta_t = (Delta_s * Delta_s) / (4.0 * C_DIFFUSION)
 */

#endif
