#!/usr/bin/env bash
# run_viz.sh  –  Build venv (once) then launch the visualizer
# Usage: ./run_viz.sh [visualize.py args...]
set -e
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
VENV="$SCRIPT_DIR/.venv"

if [ ! -f "$VENV/bin/activate" ]; then
	echo "[run_viz] Creating virtual environment..."
	python3 -m venv "$VENV"
	"$VENV/bin/pip" install --quiet -r "$SCRIPT_DIR/requirements.txt"
fi

"$VENV/bin/python3" "$SCRIPT_DIR/visualize.py" "$@"
