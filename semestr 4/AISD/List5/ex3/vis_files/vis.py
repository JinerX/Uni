import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def visualize_binomial_heap_results(n=500):
    """
    Visualize the results of binomial heap experiments
    """
    
    # Read the main results file
    try:
        df = pd.read_csv(f'binomial_heap_results_n{n}.csv')
        summary_df = pd.read_csv(f'binomial_heap_summary_n{n}.csv')
    except FileNotFoundError:
        print(f"Results files for n={n} not found. Please run the C++ program first.")
        return
    
    # Set up the plotting
    fig, axes = plt.subplots(1, 2, figsize=(12, 5))
    fig.suptitle(f'Binomial Heap Analysis (n={n})', fontsize=14, fontweight='bold')
    
    # Plot 1: Cumulative comparisons over time
    ax1 = axes[0]
    cumulative_cols = [col for col in df.columns if 'Cumulative' in col]
    
    for i, cum_col in enumerate(cumulative_cols):
        cum_data = df[cum_col].dropna()
        ax1.plot(range(1, len(cum_data) + 1), cum_data, 
                label=f'Experiment {i+1}', linewidth=2)
    
    ax1.set_xlabel('Operation Number')
    ax1.set_ylabel('Cumulative Comparisons')
    ax1.set_title('Cumulative Comparisons Over Time')
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    
    # Plot 2: Total comparisons per experiment
    ax2 = axes[1]
    exp_numbers = summary_df['Experiment']
    total_comps = summary_df['Total_Comparisons']
    
    bars = ax2.bar(exp_numbers, total_comps, 
                   color=['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd'])
    ax2.set_xlabel('Experiment Number')
    ax2.set_ylabel('Total Comparisons')
    ax2.set_title('Total Comparisons per Experiment')
    ax2.grid(True, alpha=0.3, axis='y')
    
    # Add value labels on bars
    for bar, value in zip(bars, total_comps):
        ax2.text(bar.get_x() + bar.get_width()/2, bar.get_height() + max(total_comps)*0.01,
                f'{value}', ha='center', va='bottom', fontweight='bold')
    
    plt.tight_layout()
    plt.savefig(f'binomial_heap_analysis_n{n}.png', dpi=300, bbox_inches='tight')
    plt.show()
    
    # Print summary statistics
    print(f"\n=== Analysis Summary for n={n} ===")
    print(f"Number of experiments: {len(summary_df)}")
    print(f"Average total comparisons: {summary_df['Total_Comparisons'].mean():.2f}")
    print(f"Standard deviation: {summary_df['Total_Comparisons'].std():.2f}")
    print(f"Min total comparisons: {summary_df['Total_Comparisons'].min()}")
    print(f"Max total comparisons: {summary_df['Total_Comparisons'].max()}")
    print(f"Range: {summary_df['Total_Comparisons'].max() - summary_df['Total_Comparisons'].min()}")
    
    # Check if all experiments produced sorted results
    all_sorted = all(summary_df['Is_Sorted'] == 'YES')
    all_empty = all(summary_df['Heap_Empty'] == 'YES')
    print(f"\nVerification:")
    print(f"All sequences sorted: {all_sorted}")
    print(f"All heaps empty after extraction: {all_empty}")

if __name__ == "__main__":
    # Visualize results for n=500
    visualize_binomial_heap_results(500)