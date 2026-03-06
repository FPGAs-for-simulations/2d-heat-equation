#!/usr/bin/env python3
"""
visualize_hls.py  –  Visualiser for heat2d HLS testbench output
================================================================
Thin wrapper: the HLS testbench writes snapshots and metrics CSVs
in the SAME format as the original heat2d runner, so we simply
call the original visualize.py with the correct --outdir.

Usage:
    python3 visualize_hls.py [options]
    ./run_viz_hls.sh [options]

Options (forwarded to visualize.py):
    --outdir   DIR   HLS output dir  (default: output_hls)
    --mode     MODE  serial | parallel | both
    --fps      N     animation fps
    --save           save figures instead of showing
    --no-anim        skip animation
    --steps    N     max steps in timing plot
    --savedir  DIR   where to save figures

Additional HLS-only notes printed before forwarding:
  * csim timing ≠ FPGA timing; synthesis report is authoritative.
  * Parallelism on FPGA = spatial unrolling (NUM_PE rows/cycle).
"""

import sys
import os
import subprocess

# Find the original visualize.py relative to this file
_HERE = os.path.dirname(os.path.abspath(__file__))
_VIZ  = os.path.join(_HERE, '..', 'heat2d', 'visualize.py')
_VIZ  = os.path.normpath(_VIZ)

def main():
    args = sys.argv[1:]

    # Set default outdir if not provided
    if '--outdir' not in args:
        args = ['--outdir', 'output_hls'] + args

    print("=" * 60)
    print("  heat2d HLS visualiser")
    print("  Forwarding to:", _VIZ)
    print("  NOTE: csim timing is CPU-based (software emulation).")
    print("        FPGA cycle count: see Vitis HLS synthesis report.")
    print("=" * 60)
    print()

    # Use the venv Python from heat2d if available
    _VENV_PY = os.path.join(_HERE, '..', 'heat2d', '.venv', 'bin', 'python3')
    _VENV_PY = os.path.normpath(_VENV_PY)

    python = _VENV_PY if os.path.exists(_VENV_PY) else sys.executable

    cmd = [python, _VIZ] + args
    result = subprocess.run(cmd)
    sys.exit(result.returncode)

if __name__ == '__main__':
    main()
