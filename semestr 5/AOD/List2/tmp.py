import networkx as nx
import matplotlib.pyplot as plt

# Define the edges: (from, to, cost, time)
edges = [
    (1,2,2,7),
    (1,3,10,8),
    (2,3,2,7)
]

# Create a directed graph
G = nx.DiGraph()

# Add edges with attributes
for u, v, cost, time in edges:
    G.add_edge(u, v, cost=cost, time=time)

# Choose layout (you can try others like spring_layout or circular_layout)
pos = nx.spring_layout(G, seed=42)

# Draw the graph
plt.figure(figsize=(10, 6))
nx.draw(
    G, pos,
    with_labels=True,
    node_size=700,
    node_color="lightblue",
    font_weight="bold",
    arrowsize=20
)

# Prepare edge labels
edge_labels = {(u, v): f"c={d['cost']}, t={d['time']}" for u, v, d in G.edges(data=True)}

# Draw edge labels
nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, font_color="red")

# Save to file
plt.title("Directed Graph with Cost and Time")
plt.tight_layout()
plt.savefig("graph2.png", dpi=300)
plt.show()
