// ============================================================
// main.cpp  –  Demo runner
// ============================================================
// Runs both solvers with paper example parameters and prints
// a summary.  Tweak HeatConfig values below or use CLI args.
// ============================================================
#include "include/config.hpp"
#include "include/runner.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>

using namespace heat2d;

// ---- Simple CLI parsing ------------------------------------
static void usage(const char *prog) {
  std::printf(
      "Usage: %s [options]\n"
      "  -N <int>        Interior grid points per dim  (default: 30)\n"
      "  -steps <int>    Time steps                    (default: 100000)\n"
      "  -c <double>     Thermal diffusivity           (default: 0.1)\n"
      "  -threads <int>  Threads for parallel solver   (default: 4)\n"
      "  -snap <int>     Snapshot interval (0=off)     (default: 500)\n"
      "  -outdir <str>   Output directory              (default: output)\n"
      "  -random         Use random initial condition\n"
      "  -ic <double>    Uniform interior IC value     (default: 0.0)\n"
      "  -bl <double>    BC left                       (default: 10.0)\n"
      "  -br <double>    BC right                      (default: 40.0)\n"
      "  -bb <double>    BC bottom                     (default: 30.0)\n"
      "  -bt <double>    BC top                        (default: 50.0)\n"
      "  -h              Print this help\n",
      prog);
}

int main(int argc, char *argv[]) {
  HeatConfig cfg;
  // Override defaults to something that takes real time and shows parallel
  // speedup. Paper's "medium" case: ~565x565 grid (N=563) at 10000 steps ≈ 60s
  // serial (paper Table 1). We use N=500 / 5000 steps as a comfortable ~5-15s
  // range on modern hardware.
  cfg.N = 500;
  cfg.num_steps = 100000;
  cfg.num_threads = std::max(1u, std::thread::hardware_concurrency());
  cfg.snapshot_interval = 500;

  for (int i = 1; i < argc; ++i) {
    if (!std::strcmp(argv[i], "-h")) {
      usage(argv[0]);
      return 0;
    } else if (!std::strcmp(argv[i], "-N") && i + 1 < argc)
      cfg.N = std::atoi(argv[++i]);
    else if (!std::strcmp(argv[i], "-steps") && i + 1 < argc)
      cfg.num_steps = std::atoi(argv[++i]);
    else if (!std::strcmp(argv[i], "-c") && i + 1 < argc)
      cfg.c = std::atof(argv[++i]);
    else if (!std::strcmp(argv[i], "-threads") && i + 1 < argc)
      cfg.num_threads = std::atoi(argv[++i]);
    else if (!std::strcmp(argv[i], "-snap") && i + 1 < argc)
      cfg.snapshot_interval = std::atoi(argv[++i]);
    else if (!std::strcmp(argv[i], "-outdir") && i + 1 < argc)
      cfg.output_dir = argv[++i];
    else if (!std::strcmp(argv[i], "-random"))
      cfg.use_random_ic = true;
    else if (!std::strcmp(argv[i], "-ic") && i + 1 < argc)
      cfg.ic_value = std::atof(argv[++i]);
    else if (!std::strcmp(argv[i], "-bl") && i + 1 < argc)
      cfg.bc_left = std::atof(argv[++i]);
    else if (!std::strcmp(argv[i], "-br") && i + 1 < argc)
      cfg.bc_right = std::atof(argv[++i]);
    else if (!std::strcmp(argv[i], "-bb") && i + 1 < argc)
      cfg.bc_bottom = std::atof(argv[++i]);
    else if (!std::strcmp(argv[i], "-bt") && i + 1 < argc)
      cfg.bc_top = std::atof(argv[++i]);
    else {
      std::fprintf(stderr, "Unknown option: %s\n", argv[i]);
      usage(argv[0]);
      return 1;
    }
  }

  std::printf("heat2d runner\n");
  std::printf("  N=%d  steps=%d  c=%.3f  dt=%.6f  ds=%.6f  r=%.4f\n", cfg.N,
              cfg.num_steps, cfg.c, cfg.dt(), cfg.ds(), cfg.r());
  std::printf("  BCs: left=%.1f right=%.1f bottom=%.1f top=%.1f\n", cfg.bc_left,
              cfg.bc_right, cfg.bc_bottom, cfg.bc_top);
  std::printf("  threads(parallel)=%d  snapshot_interval=%d  output=%s\n\n",
              cfg.num_threads, cfg.snapshot_interval, cfg.output_dir.c_str());

  RunnerResult res = run_heat2d(cfg);
  print_summary(res);

  return 0;
}
