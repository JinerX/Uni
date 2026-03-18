from pathlib import Path
import numpy as np
from math import sqrt
import matplotlib.pyplot as plt

p = Path('./data')



# , labels : np.array

def plot_path(path : np.array, name : str):
    plt.figure(figsize=(6,6))
    plt.plot(path[:,0], path[:,1], 'o-')

    plt.plot([path[-1,0], path[0,0]],
             [path[-1,1], path[0,1]])
    # for i, (x, y) in enumerate(path):
        # plt.text(x, y, str(labels[i]), fontsize=9)
    plt.gca().set_aspect('equal')
    plt.savefig(name)





# Prims algorithm:
# for each iteration
# pick closest unvisited node
# update distances
# repeat until |V|-1 edges have been selected

def Prim(nodes):
    n = len(nodes)
    start_node_idx = 0
    closest = np.zeros((n,)) 
    closest[0] -= 1 # we already have node 0
    distances = np.zeros((n,))
    distances[0] = np.inf 
    edges = []
    weight = 0
    for i in range(n):
        distances[i] = sqrt((nodes[i][0] - nodes[0][0]) ** 2 + (nodes[i][1] - nodes[0][1])**2)
    distances[0] = np.inf
    
    # we don't have to check for unconnected graphs, because we're dealing with full graphs here
    while(len(edges) < n-1):
        next_id = np.argmin(distances)
        if closest[next_id] == -1:
            raise ValueError("Unexpected behaviour node which was already visited has been selected")
        edges.append((int(closest[next_id]), int(next_id), float(distances[next_id])))
        closest[next_id] = -1
        weight += float(distances[next_id])
        distances[next_id] = np.inf
        for i in range(n):
            if closest[i] == -1:
                continue
            # distance between our new node and all the others
            dx = nodes[i][0] - nodes[next_id][0]
            dy = nodes[i][1] - nodes[next_id][1]
            dist = sqrt(dx ** 2 + dy ** 2)
            if distances[i] == np.inf:
                raise ValueError("Node has distance infinity despite being marked as unvisited")
            if distances[i] > dist:
                distances[i] = dist
                closest[i] = next_id
    return edges, weight

def DFS(adjacency_list):
    start = 0

    path = []
    visited = set()
    stack = [start]
    while stack:
        node = stack.pop()
        if node not in visited:
            visited.add(node)
            path.append(node)
            for neighbor in reversed(adjacency_list[node]):
                if neighbor not in visited:
                    stack.append(neighbor)
    
    return path



for file in p.iterdir():
    print(f"file: {str(file)}")
    nodes = []

    with open(file, 'r') as f:
        for line in f:
            words = line.split(" ")
            first_word = words[0]
            if (first_word.isdigit()):
                nodes.append((float(words[1]), float(words[2])))

    nodes = np.array(nodes)

    edges, weight = Prim(nodes)
    print(f"Weight of MST: {weight}\n")
    
    # Generating heuristic

    # create adjacency list

    adjacency_list = [[] for _ in range(len(nodes))]

    for edge in edges:
        fr = edge[0]
        to = edge[1]
        
        # undirected so append 2 values
        adjacency_list[fr].append(to)
        adjacency_list[to].append(fr)
    
    path = DFS(adjacency_list)
    p = path[1:]
    p.append(path[0])

    path_edges = list(zip(path,p))

    # calculate the cost
    dfs_path_cost = 0
    for from_idx in range(len(path)):
        to_idx = (from_idx + 1) % len(path)
        from_node = path[from_idx]
        to_node = path[to_idx]
        dx = (nodes[from_node][0] - nodes[to_node][0]) ** 2
        dy = (nodes[from_node][1] - nodes[to_node][1]) ** 2
        weight = sqrt(dx + dy)
        dfs_path_cost += weight

    print(f"Weight of the path which used DFS as heuristic: {dfs_path_cost}")
    plot_path(path=np.array(path_edges), name=f"DFS_heuristic_f={str(file).split('/')[1]}_weight={dfs_path_cost}_no_labels.png")


# FILE: data/uy734.tsp


# =========GROUPING============
#  group_n: 10



# number of mins: (100,)

#  AVG of mins:
# 1599684.0633452707



# =========GROUPING============
#  group_n: 50



# number of mins: (20,)

#  AVG of mins:
# 1580002.5548889958

# =========+MINIMUM=========
# TRUE_MIN: 1551979.0515724104

# file: data/uy734.tsp
# Weight of MST: 69876.4675185079

# Weight of the path which used DFS as heuristic: 172573.0289851902