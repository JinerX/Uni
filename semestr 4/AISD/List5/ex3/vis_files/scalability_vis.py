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

def visualize_scalability_results():
    """
    Visualize scalability test results
    """
    try:
        df = pd.read_csv('scalability_results.csv')
    except FileNotFoundError:
        print("Scalability results file not found. Please run the C++ program first.")
        return
    
    # Create scalability plot
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))
    fig.suptitle('Binomial Heap Scalability Analysis', fontsize=14, fontweight='bold')
    
    # Plot 1: Total comparisons vs n
    ax1.plot(df['n'], df['avg_total_comparisons'], 'bo-', linewidth=2, markersize=6)
    ax1.fill_between(df['n'], 
                     df['avg_total_comparisons'] - df['std_dev'],
                     df['avg_total_comparisons'] + df['std_dev'],
                     alpha=0.3)
    ax1.set_xlabel('n (heap size)')
    ax1.set_ylabel('Average Total Comparisons')
    ax1.set_title('Total Comparisons vs n')
    ax1.grid(True, alpha=0.3)
    
    # Add theoretical curve
    theoretical_total = df['n'] * np.log2(df['n']) * 6  # rough estimate
    ax1.plot(df['n'], theoretical_total, 'r--', alpha=0.7, 
             label='Theoretical O(n log n)')
    ax1.legend()
    
    # Plot 2: Average cost per n
    ax2.plot(df['n'], df['avg_per_n'], 'go-', linewidth=2, markersize=6)
    ax2.set_xlabel('n (heap size)')
    ax2.set_ylabel('Average Comparisons per n')
    ax2.set_title('Average Cost per Element vs n')
    ax2.grid(True, alpha=0.3)
    
    # Add theoretical curve for average cost
    theoretical_avg = np.log2(df['n']) * 6  # rough estimate
    ax2.plot(df['n'], theoretical_avg, 'r--', alpha=0.7,
             label='Theoretical O(log n)')
    ax2.legend()
    
    plt.tight_layout()
    plt.savefig('binomial_heap_scalability.png', dpi=300, bbox_inches='tight')
    plt.show()
    
    # Print analysis
    print(f"\n=== Scalability Analysis ===")
    print(f"Tested n values: {df['n'].min()} to {df['n'].max()}")
    print(f"Average cost per n:")
    print(f"  Min: {df['avg_per_n'].min():.2f}")
    print(f"  Max: {df['avg_per_n'].max():.2f}")
    print(f"  At n=100: {df[df['n']==100]['avg_per_n'].iloc[0]:.2f}")
    print(f"  At n=1000: {df[df['n']==1000]['avg_per_n'].iloc[0]:.2f}")
    print(f"  At n=10000: {df[df['n']==10000]['avg_per_n'].iloc[0]:.2f}")
    
    # Growth analysis
    n100_cost = df[df['n']==100]['avg_per_n'].iloc[0]
    n1000_cost = df[df['n']==1000]['avg_per_n'].iloc[0]
    n10000_cost = df[df['n']==10000]['avg_per_n'].iloc[0]
    
    print(f"\nGrowth analysis:")
    print(f"  Cost ratio (n=1000 vs n=100): {n1000_cost/n100_cost:.2f}")
    print(f"  Cost ratio (n=10000 vs n=1000): {n10000_cost/n1000_cost:.2f}")
    print(f"  Theoretical log2(1000)/log2(100): {np.log2(1000)/np.log2(100):.2f}")
    print(f"  Theoretical log2(10000)/log2(1000): {np.log2(10000)/np.log2(1000):.2f}")

if __name__ == "__main__":
    # Visualize results for n=500
    visualize_binomial_heap_results(500)
    
    # Visualize scalability results
    visualize_scalability_results()