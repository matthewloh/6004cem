import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np  # For standard deviation calculation

# --- Configuration ---
INPUT_CSV = 'results.csv'
OUTPUT_PLOT_COMPARISON = 'performance_comparison.png'
OUTPUT_PLOT_THRESHOLD = 'threshold_effect.png'
# The initial threshold used in ConditionalParallelInitial
INITIAL_THRESHOLD_VALUE = 5000
MATRIX_SIZE = 1000  # Should match the N in the C++ code

# --- Data Loading and Processing ---


def load_and_process_data(filepath):
    """Loads the CSV data and calculates mean and std dev."""
    try:
        df = pd.read_csv(filepath)
    except FileNotFoundError:
        print(f"Error: Input file '{filepath}' not found.")
        print("Please run the C++ experiment first using './run_experiment.sh'")
        exit(1)

    # Calculate mean and standard deviation for each group
    # Group by Method, Size, and Threshold
    summary = df.groupby(['Method', 'Size', 'Threshold'])[
        'Time'].agg(['mean', 'std']).reset_index()
    # Fill NaN in std dev (happens with 1 trial, though we have 10) with 0
    summary['std'] = summary['std'].fillna(0)
    return df, summary

# --- Plotting Functions ---


def plot_method_comparison(summary_df, initial_threshold, matrix_size, output_filename):
    """Plots a bar chart comparing Sequential, Forced Parallel, and Initial Conditional Parallel."""
    # Filter data for the main comparison (Seq, Forced, CondInitial at specific N)
    comparison_methods = ['Sequential',
                          'ForcedParallel', 'ConditionalParallelInitial']
    plot_data = summary_df[
        (summary_df['Method'].isin(comparison_methods)) &
        (summary_df['Size'] == matrix_size)
        # No need to filter by threshold for Seq/Forced as it's 0
        # For CondInitial, the threshold is specific
    ].copy()  # Use copy to avoid SettingWithCopyWarning

    # Ensure the CondInitial row uses the correct initial threshold for selection if needed
    # plot_data = plot_data[(plot_data['Method'] != 'ConditionalParallelInitial') | (plot_data['Threshold'] == initial_threshold)]
    # Actually, the groupby already handled this. Let's ensure the labels are clear.

    plt.figure(figsize=(10, 6))
    sns.barplot(x='Method', y='mean', data=plot_data, ci=None,
                palette='viridis')  # Disable seaborn error bars initially

    # Add error bars manually using std dev
    plt.errorbar(x=plot_data['Method'], y=plot_data['mean'], yerr=plot_data['std'],
                 fmt='none', c='black', capsize=5, label='Std Dev')

    plt.title(
        f'Matrix Multiplication Performance Comparison (N={matrix_size})')
    plt.xlabel('Multiplication Method')
    plt.ylabel('Average Time (seconds)')
    # Prettier labels
    method_labels = {
        'Sequential': 'Sequential',
        'ForcedParallel': 'Forced Parallel',
        'ConditionalParallelInitial': f'Conditional Parallel\n(Threshold={initial_threshold})'
    }
    plt.xticks(ticks=range(len(plot_data['Method'])), labels=[
               method_labels.get(m, m) for m in plot_data['Method']])
    plt.grid(axis='y', linestyle='--', alpha=0.7)
    plt.tight_layout()
    plt.savefig(output_filename)
    print(f"Comparison plot saved to {output_filename}")
    plt.close()


def plot_threshold_effect(summary_df, matrix_size, output_filename):
    """Plots the effect of the threshold on Conditional Parallel performance."""
    # Filter data for the threshold analysis (CondThreshold at specific N)
    plot_data = summary_df[
        (summary_df['Method'] == 'ConditionalParallelThreshold') &
        (summary_df['Size'] == matrix_size)
    ].sort_values('Threshold')

    if plot_data.empty:
        print("Warning: No data found for 'ConditionalParallelThreshold'. Cannot generate threshold plot.")
        return

    plt.figure(figsize=(10, 6))
    plt.errorbar(plot_data['Threshold'], plot_data['mean'], yerr=plot_data['std'],
                 fmt='-o', linestyle='-', marker='o', capsize=5, label='Average Time +/- Std Dev')

    plt.title(
        f'Effect of Threshold on Conditional Parallel Performance (N={matrix_size})')
    plt.xlabel('Parallelism Threshold (Number of Elements = Rows * Cols)')
    plt.ylabel('Average Time (seconds)')
    plt.xscale('log')  # Thresholds span orders of magnitude, log scale is good
    plt.grid(True, which='both', linestyle='--', alpha=0.7)
    plt.legend()
    plt.tight_layout()
    plt.savefig(output_filename)
    print(f"Threshold effect plot saved to {output_filename}")
    plt.close()


# --- Main Execution ---
if __name__ == "__main__":
    print("Loading and processing data...")
    full_df, summary_df = load_and_process_data(INPUT_CSV)

    print("Generating plots...")
    # Plot 1: Comparison of main methods
    plot_method_comparison(summary_df, INITIAL_THRESHOLD_VALUE,
                           MATRIX_SIZE, OUTPUT_PLOT_COMPARISON)

    # Plot 2: Effect of threshold on conditional parallel
    plot_threshold_effect(summary_df, MATRIX_SIZE, OUTPUT_PLOT_THRESHOLD)

    print("Visualization script finished.")
