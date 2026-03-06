# ============================================================
# Makefile  –  Build and run the heat2d HLS testbench with GCC
# ============================================================
# No Vitis installation required for C simulation (csim).
# All #pragma HLS directives are silently ignored by GCC.
#
# Targets:
#   make              – build testbench binary
#   make run          – build and run csim
#   make viz          – run visualiser on output_hls/
#   make clean        – remove build artefacts
#   make clean_output – remove output_hls/ snapshots
# ============================================================

CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra \
            -Iinclude -Itb

# Source files (kernel + testbench)
SRCS := src/heat2d_serial_hls.cpp \
        src/heat2d_parallel_hls.cpp \
        tb/tb_heat2d_hls.cpp

TARGET := heat2d_hls_tb

VENV_PYTHON := ..\/heat2d\/.venv/bin/python3

.PHONY: all run viz clean clean_output

all: $(TARGET)

$(TARGET): $(SRCS) include/*.hpp tb/*.hpp
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS) -lpthread

run: $(TARGET)
	./$(TARGET)

viz: run
	$(VENV_PYTHON) visualize_hls.py --outdir output_hls --save --no-anim
	@echo "Plots saved to output_hls/plots/"

clean:
	rm -f $(TARGET)

clean_output:
	rm -rf output_hls/
