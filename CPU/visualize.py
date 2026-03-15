import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import os

# Ensure data exists
if not os.path.exists('heat_history.csv'):
    print("Error: heat_history.csv not found. Run solver first.")
    exit(1)

# Load data
df = pd.read_csv('heat_history.csv')
steps = sorted(df['step'].unique())
x_vals = sorted(df['x'].unique())
y_vals = sorted(df['y'].unique())
n_x, n_y = len(x_vals), len(y_vals)

def get_grid(step):
    # Pivot for efficient reshaping: ensures correct mapping of x/y to matrix
    grid_df = df[df['step'] == step].pivot(index='y', columns='x', values='temperature')
    return grid_df.values

# 1. Animation
print("Generating animation...")
fig, ax = plt.subplots(figsize=(8, 6))
# Using pcolormesh needs X, Y grids or unique values
X, Y = np.meshgrid(x_vals, y_vals)
im = ax.pcolormesh(X, Y, get_grid(steps[0]), cmap='hot', shading='auto', vmin=0, vmax=50)
plt.colorbar(im, label='Temperature')
title = ax.set_title(f'Heat Evolution (Step: {steps[0]})')

def update(i):
    im.set_array(get_grid(steps[i]).flatten())
    title.set_text(f'Heat Evolution (Step: {steps[i]})')
    return [im, title]

ani = animation.FuncAnimation(fig, update, frames=len(steps), interval=100, blit=True)
try:
    ani.save('evolution.gif', writer='pillow')
    print("Animation saved as evolution.gif")
except Exception as e:
    print(f"Could not save animation: {e}")

# 2. Final State Analysis
print("Generating analysis plot...")
fig1, ax1 = plt.subplots(figsize=(8, 6))
final_grid = get_grid(steps[-1])
im1 = ax1.pcolormesh(X, Y, final_grid, cmap='hot', shading='auto')
plt.colorbar(im1, ax=ax1, label='Temperature')
ax1.set_title(f'Final Steady State (Step: {steps[-1]})')
ax1.set_xlabel('x')
ax1.set_ylabel('y')

plt.tight_layout()
plt.savefig('analysis.png')
print("Analysis plot saved as analysis.png")

# Optional: show if possible (e.g., in a notebook or with a GUI)
plt.show()
