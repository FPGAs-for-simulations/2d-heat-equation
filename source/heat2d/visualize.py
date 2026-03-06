#!/usr/bin/env python3
"""
visualize.py  –  Visualiser for heat2d simulation output
=========================================================
Produces:
  1. Animated heatmap of the temperature field evolution
     (serial, parallel, or both side-by-side)
  2. Per-step timing comparison (compute_ns, comm_ns, total_ns)
  3. Aggregate bar chart: total compute / comm / overhead

Usage:
    python3 visualize.py [options]

Options:
    --outdir  DIR     heat2d output directory   (default: output)
    --mode    MODE    serial | parallel | both  (default: both)
    --fps     N       animation frames/s        (default: 10)
    --save            save figures to PNG/GIF instead of showing
    --savedir DIR     where to save figures     (default: outdir/plots)
    --steps   N       max steps to show in timing plots (0=all)
    --no-anim         skip animation, only timing plots
"""

import argparse
import glob
import os
import sys
import re

import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.colors import Normalize
from mpl_toolkits.axes_grid1 import make_axes_locatable

# ------------------------------------------------------------------ #
# Helpers                                                             #
# ------------------------------------------------------------------ #

def parse_snapshot(filepath):
    """Return (grid_array, step, sim_time) from a snapshot CSV."""
    with open(filepath) as f:
        header = f.readline()  # # rows=R cols=C step=S time=T
        m = re.search(r'rows=(\d+).*cols=(\d+).*step=(-?\d+).*time=([\d.e+\-]+)', header)
        if not m:
            raise ValueError(f"Bad header in {filepath}: {header!r}")
        rows, cols = int(m.group(1)), int(m.group(2))
        step = int(m.group(3))
        sim_time = float(m.group(4))
        data = np.loadtxt(f, delimiter=',')
    if data.shape != (rows, cols):
        raise ValueError(f"Shape mismatch in {filepath}: expected {rows}x{cols}, got {data.shape}")
    return data, step, sim_time


def load_snapshots(outdir, prefix):
    """Load all snapshot CSVs for a given prefix, sorted by step."""
    pattern = os.path.join(outdir, prefix, "step_*.csv")
    files = sorted(glob.glob(pattern))
    if not files:
        return [], [], []
    grids, steps, times = [], [], []
    for fp in files:
        g, s, t = parse_snapshot(fp)
        grids.append(g)
        steps.append(s)
        times.append(t)
    return grids, steps, times


def load_metrics(outdir, prefix):
    """Load metrics CSV, return (steps_arr, compute_ns, comm_ns, total_ns, meta)."""
    path = os.path.join(outdir, f"{prefix}_metrics.csv")
    if not os.path.exists(path):
        return None
    meta = {}
    data_lines = []
    with open(path) as f:
        for line in f:
            if line.startswith('#'):
                for kv in re.findall(r'(\w+)=([\d.\-e]+)', line):
                    meta[kv[0]] = float(kv[1])
            elif line.startswith('step'):
                pass  # header
            else:
                parts = line.strip().split(',')
                if len(parts) == 4:
                    data_lines.append([float(p) for p in parts])
    if not data_lines:
        return None
    arr = np.array(data_lines)
    return arr[:, 0], arr[:, 1], arr[:, 2], arr[:, 3], meta


# ------------------------------------------------------------------ #
# Animation                                                           #
# ------------------------------------------------------------------ #

def make_animation(grids_s, steps_s, times_s,
                   grids_p, steps_p, times_p,
                   mode, fps, save, savedir):
    """Animated heatmap side by side (or single)."""

    show_serial   = mode in ('serial',   'both') and len(grids_s) > 0
    show_parallel = mode in ('parallel', 'both') and len(grids_p) > 0

    if not show_serial and not show_parallel:
        print("No snapshot data found for animation.")
        return

    ncols = (1 if show_serial else 0) + (1 if show_parallel else 0)
    fig, axes = plt.subplots(1, ncols, figsize=(6 * ncols, 5))
    if ncols == 1:
        axes = [axes]

    # Determine global colour range across all frames
    all_data = ([g for g in grids_s] if show_serial else []) + \
               ([g for g in grids_p] if show_parallel else [])
    vmin = min(g.min() for g in all_data)
    vmax = max(g.max() for g in all_data)
    norm = Normalize(vmin=vmin, vmax=vmax)

    ims = []
    titles = []
    ax_idx = 0

    def setup_ax(ax, title, first_grid):
        im = ax.imshow(first_grid, origin='lower', norm=norm,
                       cmap='hot', aspect='equal', interpolation='nearest')
        ax.set_xlabel("y-axis (j)")
        ax.set_ylabel("x-axis (i)")
        divider = make_axes_locatable(ax)
        cax = divider.append_axes("right", size="5%", pad=0.05)
        fig.colorbar(im, cax=cax, label="Temperature")
        return im

    if show_serial:
        im_s = setup_ax(axes[ax_idx], "Serial", grids_s[0])
        t_s  = axes[ax_idx].set_title(f"Serial  step=0  t=0.000")
        ims.append(im_s); titles.append((t_s, 'S'))
        ax_idx += 1

    if show_parallel:
        im_p = setup_ax(axes[ax_idx], "Parallel", grids_p[0])
        t_p  = axes[ax_idx].set_title(f"Parallel step=0  t=0.000")
        ims.append(im_p); titles.append((t_p, 'P'))
        ax_idx += 1

    fig.suptitle("2-D Heat Equation – Temperature Field Evolution", fontsize=13)
    plt.tight_layout()

    # Match frame counts
    n_frames_s = len(grids_s) if show_serial   else 0
    n_frames_p = len(grids_p) if show_parallel else 0
    n_frames   = max(n_frames_s, n_frames_p)

    def update(frame):
        artists = []
        ti = 0
        if show_serial:
            fi = min(frame, n_frames_s - 1)
            ims[ti].set_data(grids_s[fi])
            titles[ti][0].set_text(
                f"Serial  step={steps_s[fi]}  t={times_s[fi]:.4f}")
            artists += [ims[ti], titles[ti][0]]
            ti += 1
        if show_parallel:
            fi = min(frame, n_frames_p - 1)
            ims[ti].set_data(grids_p[fi])
            titles[ti][0].set_text(
                f"Parallel step={steps_p[fi]}  t={times_p[fi]:.4f}")
            artists += [ims[ti], titles[ti][0]]
        return artists

    ani = animation.FuncAnimation(fig, update, frames=n_frames,
                                  interval=int(1000 / fps), blit=True)

    if save:
        os.makedirs(savedir, exist_ok=True)
        path = os.path.join(savedir, "heatmap_animation.gif")
        print(f"Saving animation → {path}")
        ani.save(path, writer='pillow', fps=fps)
        plt.close(fig)
    else:
        plt.show()


# ------------------------------------------------------------------ #
# Timing plots                                                        #
# ------------------------------------------------------------------ #

def plot_timing(outdir, mode, max_steps, save, savedir):
    """Per-step timing and aggregate comparison."""

    met_s = load_metrics(outdir, 'serial')   if mode in ('serial',   'both') else None
    met_p = load_metrics(outdir, 'parallel') if mode in ('parallel', 'both') else None

    if met_s is None and met_p is None:
        print("No metrics CSV files found – skipping timing plots.")
        return

    fig, axes = plt.subplots(2, 2, figsize=(14, 9))
    fig.suptitle("heat2d Performance Metrics", fontsize=14)

    # ---- Per-step compute time --------------------------------
    ax = axes[0, 0]
    ax.set_title("Per-step compute time (ns)")
    ax.set_xlabel("Step")
    ax.set_ylabel("compute_ns")

    if met_s:
        steps_arr, comp, comm, tot, _ = met_s
        sl = slice(None) if max_steps == 0 else slice(0, max_steps)
        ax.plot(steps_arr[sl], comp[sl], lw=0.8, label='Serial compute', color='steelblue')

    if met_p:
        steps_arr, comp, comm, tot, _ = met_p
        sl = slice(None) if max_steps == 0 else slice(0, max_steps)
        ax.plot(steps_arr[sl], comp[sl], lw=0.8, label='Parallel compute', color='tomato')
        ax.plot(steps_arr[sl], comm[sl], lw=0.8, label='Parallel comm/barrier', color='orange',
                linestyle='--')

    ax.legend(fontsize=8)
    ax.grid(True, alpha=0.3)

    # ---- Per-step total time ----------------------------------
    ax = axes[0, 1]
    ax.set_title("Per-step total time (ns)")
    ax.set_xlabel("Step")
    ax.set_ylabel("total_ns")

    if met_s:
        steps_arr, comp, comm, tot, _ = met_s
        sl = slice(None) if max_steps == 0 else slice(0, max_steps)
        ax.plot(steps_arr[sl], tot[sl], lw=0.8, label='Serial total', color='steelblue')

    if met_p:
        steps_arr, comp, comm, tot, _ = met_p
        sl = slice(None) if max_steps == 0 else slice(0, max_steps)
        ax.plot(steps_arr[sl], tot[sl], lw=0.8, label='Parallel total', color='tomato')

    ax.legend(fontsize=8)
    ax.grid(True, alpha=0.3)

    # ---- Histogram of per-step compute time ------------------
    ax = axes[1, 0]
    ax.set_title("Histogram: per-step compute time (ns)")
    ax.set_xlabel("compute_ns")
    ax.set_ylabel("Count")

    if met_s:
        _, comp, _, _, _ = met_s
        ax.hist(comp, bins=60, alpha=0.6, label='Serial', color='steelblue')

    if met_p:
        _, comp, _, _, _ = met_p
        ax.hist(comp, bins=60, alpha=0.6, label='Parallel', color='tomato')

    ax.legend(fontsize=8)
    ax.grid(True, alpha=0.3)

    # ---- Aggregate bar chart ---------------------------------
    ax = axes[1, 1]
    ax.set_title("Aggregate time breakdown (ms)")
    ax.set_ylabel("Time (ms)")

    labels, compute_ms_list, comm_ms_list, overhead_ms_list = [], [], [], []

    def add_bar(label, met):
        if met is None:
            return
        _, comp, comm, tot, m = met
        tc = comp.sum() / 1e6
        tco = comm.sum() / 1e6
        tt = m.get('total_time_ns', tot.sum()) / 1e6
        oh = max(0.0, tt - tc - tco)
        labels.append(label)
        compute_ms_list.append(tc)
        comm_ms_list.append(tco)
        overhead_ms_list.append(oh)

    add_bar('Serial',   met_s)
    add_bar('Parallel', met_p)

    x = np.arange(len(labels))
    w = 0.5
    b1 = ax.bar(x, compute_ms_list,  w, label='Compute',  color='steelblue')
    b2 = ax.bar(x, comm_ms_list,     w, bottom=compute_ms_list,
                label='Comm/barrier', color='orange')
    b3 = ax.bar(x, overhead_ms_list, w,
                bottom=np.array(compute_ms_list) + np.array(comm_ms_list),
                label='Overhead', color='grey', alpha=0.6)

    ax.set_xticks(x)
    ax.set_xticklabels(labels)
    ax.legend(fontsize=8)
    ax.grid(True, alpha=0.3, axis='y')

    # annotate total
    for i, (lbl, tc, tco, oh) in enumerate(
            zip(labels, compute_ms_list, comm_ms_list, overhead_ms_list)):
        total = tc + tco + oh
        ax.text(i, total + total * 0.01, f"{total:.1f}ms", ha='center', fontsize=9)

    plt.tight_layout()

    if save:
        os.makedirs(savedir, exist_ok=True)
        path = os.path.join(savedir, "timing_metrics.png")
        print(f"Saving timing plot → {path}")
        fig.savefig(path, dpi=150)
        plt.close(fig)
    else:
        plt.show()


# ------------------------------------------------------------------ #
# Final state comparison                                              #
# ------------------------------------------------------------------ #

def plot_final_state(grids_s, steps_s, grids_p, steps_p, mode, save, savedir):
    """Plot final grid state (and difference if both available)."""

    has_s = mode in ('serial', 'both') and len(grids_s) > 0
    has_p = mode in ('parallel', 'both') and len(grids_p) > 0

    if not has_s and not has_p:
        return

    show_diff = has_s and has_p
    ncols = (1 if has_s else 0) + (1 if has_p else 0) + (1 if show_diff else 0)
    fig, axes = plt.subplots(1, ncols, figsize=(6 * ncols, 5))
    if ncols == 1:
        axes = [axes]

    fig.suptitle("Final Temperature Field", fontsize=13)
    ax_idx = 0

    def plot_one(ax, grid, title):
        im = ax.imshow(grid, origin='lower', cmap='hot',
                       aspect='equal', interpolation='nearest')
        ax.set_title(title)
        ax.set_xlabel("y"); ax.set_ylabel("x")
        divider = make_axes_locatable(ax)
        cax = divider.append_axes("right", size="5%", pad=0.05)
        fig.colorbar(im, cax=cax, label="Temperature")

    if has_s:
        plot_one(axes[ax_idx], grids_s[-1], f"Serial  (step {steps_s[-1]})")
        ax_idx += 1
    if has_p:
        plot_one(axes[ax_idx], grids_p[-1], f"Parallel (step {steps_p[-1]})")
        ax_idx += 1
    if show_diff:
        diff = np.abs(grids_s[-1] - grids_p[-1])
        im = axes[ax_idx].imshow(diff, origin='lower', cmap='viridis',
                                  aspect='equal', interpolation='nearest')
        axes[ax_idx].set_title(f"|Serial − Parallel|  max={diff.max():.2e}")
        axes[ax_idx].set_xlabel("y"); axes[ax_idx].set_ylabel("x")
        divider = make_axes_locatable(axes[ax_idx])
        cax = divider.append_axes("right", size="5%", pad=0.05)
        fig.colorbar(im, cax=cax, label="|ΔT|")

    plt.tight_layout()

    if save:
        os.makedirs(savedir, exist_ok=True)
        path = os.path.join(savedir, "final_state.png")
        print(f"Saving final state → {path}")
        fig.savefig(path, dpi=150)
        plt.close(fig)
    else:
        plt.show()


# ------------------------------------------------------------------ #
# Main                                                                #
# ------------------------------------------------------------------ #

def main():
    parser = argparse.ArgumentParser(
        description="Visualiser for heat2d C++ simulation output")
    parser.add_argument('--outdir',   default='output',
                        help='heat2d output directory')
    parser.add_argument('--mode',     default='both',
                        choices=['serial', 'parallel', 'both'])
    parser.add_argument('--fps',      type=int, default=10)
    parser.add_argument('--save',     action='store_true',
                        help='save figures instead of displaying')
    parser.add_argument('--savedir',  default=None,
                        help='where to save (default: outdir/plots)')
    parser.add_argument('--steps',    type=int, default=0,
                        help='max steps shown in timing plots (0=all)')
    parser.add_argument('--no-anim',  action='store_true',
                        help='skip animation')
    args = parser.parse_args()

    savedir = args.savedir or os.path.join(args.outdir, 'plots')

    if args.save:
        matplotlib.use('Agg')

    # Load snapshots
    grids_s, steps_s, times_s = load_snapshots(args.outdir, 'serial')
    grids_p, steps_p, times_p = load_snapshots(args.outdir, 'parallel')

    print(f"Loaded {len(grids_s)} serial snapshots, {len(grids_p)} parallel snapshots")

    # Final state comparison
    plot_final_state(grids_s, steps_s, grids_p, steps_p,
                     args.mode, args.save, savedir)

    # Timing plots
    plot_timing(args.outdir, args.mode, args.steps, args.save, savedir)

    # Animation
    if not args.no_anim:
        make_animation(grids_s, steps_s, times_s,
                       grids_p, steps_p, times_p,
                       args.mode, args.fps, args.save, savedir)


if __name__ == '__main__':
    main()
