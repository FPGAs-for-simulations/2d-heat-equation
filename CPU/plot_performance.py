import pandas as pd
import matplotlib.pyplot as plt
import os

def plot_performance():
    if not os.path.exists('performance.csv'):
        print("Error: performance.csv not found. Run solver first.")
        return

    df = pd.read_csv('performance.csv')
    
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 10))

    # Plot 1: Cumulative Time
    ax1.plot(df['step'], df['cumulative_time_ms'] / 1000.0, label='Cumulative Time', color='blue')
    ax1.set_title('Simulation Performance: Cumulative Time')
    ax1.set_xlabel('Iteration Step')
    ax1.set_ylabel('Total Time (seconds)')
    ax1.grid(True)
    ax1.legend()

    # Plot 2: Time per Step (smoothed if needed, but here raw)
    # We can also plot "Steps per second"
    steps_diff = df['step'].diff()
    time_diff = df['cumulative_time_ms'].diff()
    # To avoid division by zero or NaN at first row
    steps_per_sec = (steps_diff / (time_diff / 1000.0)).fillna(0)
    
    ax2.plot(df['step'], steps_per_sec, label='Throughput (Steps/sec)', color='green')
    ax2.set_title('Simulation Performance: Throughput')
    ax2.set_xlabel('Iteration Step')
    ax2.set_ylabel('Iterations per Second')
    ax2.grid(True)
    ax2.legend()

    plt.tight_layout()
    plt.savefig('performance_plot.png')
    print("Performance plot saved as performance_plot.png")

if __name__ == "__main__":
    plot_performance()
