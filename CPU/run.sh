#!/bin/bash

# Exit on any error
set -e

echo "Compiling..."
make clean
make

echo "Running Simulation..."
./solver

if [ -d ".venv" ]; then
    PYTHON=".venv/bin/python"
else
    PYTHON="python3"
fi

echo "Visualizing..."
$PYTHON visualize.py
$PYTHON plot_performance.py

echo "Process complete! Check evolution.gif, analysis.png, and performance_plot.png"
