#pragma once
// ============================================================
// config.hpp  –  All configurable parameters for heat2d
// ============================================================
// Every numeric knob lives here.  Derived quantities (ds, dt, r)
// are computed from the primary values so they stay consistent.
// ============================================================
#include <string>

namespace heat2d {

struct HeatConfig {
    // ---- Grid -----------------------------------------------
    int N = 30;             // interior points per dimension
                            // total grid = (N+2) x (N+2)

    // ---- Time -----------------------------------------------
    int num_steps = 10000;  // number of explicit time steps

    // ---- Physics --------------------------------------------
    double c = 0.1;         // thermal diffusivity

    // ---- Boundary conditions (constant edges) ---------------
    // Paper example from Fig.1: left=10, right=40, front=30, back=50
    double bc_left   = 10.0;   // u[0][j]   – x=0 edge
    double bc_right  = 40.0;   // u[N+1][j] – x=1 edge
    double bc_bottom = 30.0;   // u[i][0]   – y=0 edge
    double bc_top    = 50.0;   // u[i][N+1] – y=1 edge

    // ---- Initial condition for interior points --------------
    double ic_value      = 0.0;    // uniform IC; used when use_random_ic=false
    bool   use_random_ic = false;
    int    random_seed   = 42;
    double random_min    = -30.0;
    double random_max    =  30.0;

    // ---- Parallelism ----------------------------------------
    int num_threads = 4;           // threads for domain-decomp solver

    // ---- Output ---------------------------------------------
    int         snapshot_interval = 500;  // save grid every N steps; 0 = off
    std::string output_dir        = "output";
    bool        save_metrics      = true; // write metrics CSVs

    // ---- Derived (always consistent with N and c) -----------
    double ds()        const { return 1.0 / static_cast<double>(N + 1); }
    // stability: dt <= ds^2 / (2c); paper uses ds^2/(4c)
    double dt()        const { double s = ds(); return (s * s) / (4.0 * c); }
    // stencil coefficient r = c*dt/ds^2 = 0.25 with above dt
    double r()         const { return c * dt() / (ds() * ds()); }
    int    grid_size() const { return N + 2; }
};

} // namespace heat2d
