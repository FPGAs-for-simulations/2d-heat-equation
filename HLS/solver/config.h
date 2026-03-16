#ifndef CONFIG_H
#define CONFIG_H

/* Hardware Constraints - defines maximum BRAM allocation */
#define MAX_SIZE 512

/* Simulation Defaults */
#define DEFAULT_N           30
#define DEFAULT_ITERATIONS  100
#define DEFAULT_SNAPSHOT    25
#define DEFAULT_C_DIFFUSION 0.1f
#define DEFAULT_BC_LEFT     10.0f
#define DEFAULT_BC_RIGHT    40.0f
#define DEFAULT_BC_TOP      30.0f
#define DEFAULT_BC_BOTTOM   50.0f
#define DEFAULT_INITIAL_VAL 30.0f

#endif
