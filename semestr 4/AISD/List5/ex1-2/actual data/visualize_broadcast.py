import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load data
data = pd.read_csv('broadcast_analysis.csv')
print(f"Loaded data for n = {data['n'].min()} to {data['n'].max()}")

# Simple 2x2 plot
fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(12, 10))

# Plot 1: Basic line plot - Average rounds
ax1.plot(data['n'], data['avg_rounds'], 'b-o', linewidth=2)
ax1.set_xlabel('Number of vertices (n)')
ax1.set_ylabel('Average rounds')
ax1.set_title('Average Broadcast Rounds')
ax1.grid(True)

# Plot 2: Min vs Max
ax2.plot(data['n'], data['min_rounds'], 'g-s', label='Min')
ax2.plot(data['n'], data['max_rounds'], 'r-^', label='Max')
ax2.set_xlabel('Number of vertices (n)')
ax2.set_ylabel('Rounds')
ax2.set_title('Min vs Max Rounds')
ax2.legend()
ax2.grid(True)

# Plot 3: Compare with log(n)
ax3.plot(data['n'], data['avg_rounds'], 'b-o', label='Average')
ax3.plot(data['n'], np.log2(data['n']), 'r--', label='log₂(n)')
ax3.set_xlabel('Number of vertices (n)')
ax3.set_ylabel('Rounds')
ax3.set_title('Average vs log₂(n)')
ax3.legend()
ax3.grid(True)

# Plot 4: Efficiency ratio
ratio = data['max_rounds'] / data['min_rounds']
ax4.plot(data['n'], ratio, 'mo-', linewidth=2)
ax4.set_xlabel('Number of vertices (n)')
ax4.set_ylabel('Max/Min ratio')
ax4.set_title('Root Choice Efficiency')
ax4.grid(True)

plt.tight_layout()
plt.savefig('broadcast_results.png', dpi=150)
plt.show()

# Simple stats
print(f"\nSimple Statistics:")
print(f"Average rounds: {data['avg_rounds'].mean():.2f}")
print(f"Average efficiency ratio: {ratio.mean():.2f}")
print(f"Best case improvement: {((ratio.mean()-1)*100):.1f}%")