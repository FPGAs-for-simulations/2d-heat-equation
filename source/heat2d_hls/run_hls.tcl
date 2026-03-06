# ============================================================
# run_hls.tcl  –  Vitis HLS project script
# ============================================================
# Usage (from Vitis HLS command line):
#   vitis_hls -f run_hls.tcl
#
# Runs:
#   1. C simulation (csim)  – verifies functionality vs golden
#   2. C synthesis  (csynth) – generates RTL, produces timing/area report
#   3. (Optional) Co-simulation and export commented out below
#
# Edit HLS_PART and HLS_CLOCK_NS for your target board.
# ============================================================

# ---- Configuration ------------------------------------------
set PROJECT_NAME  "heat2d_hls_project"
set SOLUTION_SER  "solution_serial"
set SOLUTION_PAR  "solution_parallel"
set HLS_PART      "xczu9eg-ffvb1156-2-i"   ;# ZCU102 (Zynq UltraScale+)
# set HLS_PART   "xcu280-fsvh2892-2L-e"    ;# Alveo U280
set HLS_CLOCK_NS  "10"                      ;# 100 MHz

# Source files (relative to project root = directory containing this script)
set SRC_INCLUDES  "-Iinclude"
set KERN_SERIAL   "src/heat2d_serial_hls.cpp"
set KERN_PARALLEL "src/heat2d_parallel_hls.cpp"
set TB_FILE       "tb/tb_heat2d_hls.cpp"
set TB_FLAGS      "-Iinclude -Itb"

# ============================================================
# Helper proc: run csim + csynth for one top function
# ============================================================
proc run_solution { proj_name sol_name top_fn kern_file tb_file
                    part clock_ns src_inc tb_flags } {

    open_project $proj_name
    set_top $top_fn

    # Kernel source
    add_files $kern_file -cflags $src_inc

    # Testbench (excluded from synthesis, included in csim)
    add_files -tb $tb_file -cflags $tb_flags

    open_solution $sol_name -reset

    # Target device and clock
    set_part $part
    create_clock -period $clock_ns -name default

    # Optional: override directives for quick exploration
    # config_array_partition -auto_partition_threshold 8

    # ---- C simulation ------------------------------------------
    puts "\n[INFO] Running csim for $top_fn ..."
    csim_design -clean

    # ---- C synthesis -------------------------------------------
    puts "\n[INFO] Running csynth for $top_fn ..."
    csynth_design

    # ---- Co-simulation (uncomment when RTL is ready) -----------
    # cosim_design -rtl vhdl -trace_level all

    # ---- Export IP (uncomment to package for Vivado) -----------
    # export_design -format ip_catalog -description "heat2d HLS kernel"

    close_project
}

# ============================================================
# Run serial kernel
# ============================================================
run_solution $PROJECT_NAME $SOLUTION_SER \
    "heat2d_serial_hls" $KERN_SERIAL $TB_FILE \
    $HLS_PART $HLS_CLOCK_NS $SRC_INCLUDES $TB_FLAGS

# ============================================================
# Run parallel kernel
# ============================================================
run_solution $PROJECT_NAME $SOLUTION_PAR \
    "heat2d_parallel_hls" $KERN_PARALLEL $TB_FILE \
    $HLS_PART $HLS_CLOCK_NS $SRC_INCLUDES $TB_FLAGS

puts "\n[INFO] All done.  Check ${PROJECT_NAME}/${SOLUTION_SER}/syn/report/ for timing/area."
puts "[INFO] Parallel expected latency ≈ 1/${HLS_NUM_PE} of serial (set in hls_config.hpp)."
