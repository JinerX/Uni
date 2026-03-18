import numpy as np
from math import sqrt
from pathlib import Path
import matplotlib.pyplot as plt


def plot_path(path : np.array, name : str, labels : np.array):
    plt.figure(figsize=(6,6))
    plt.plot(path[:,0], path[:,1], 'o-')

    plt.plot([path[-1,0], path[0,0]],
             [path[-1,1], path[0,1]])
    # for i, (x, y) in enumerate(path):
        # plt.text(x, y, str(labels[i]), fontsize=9)
    plt.gca().set_aspect('equal')
    plt.savefig(name)





N_ITER = 1000
GROUP_N = 10

GROUPS_NS = [10, 50]

p = Path('./data')
for x in p.iterdir():
    print(str(x))

for file in p.iterdir():

    nodes = []

    with open(file, 'r') as f:
        for line in f:
            words = line.split(" ")
            first_word = words[0]
            if (first_word.isdigit()):
                nodes.append((float(words[1]), float(words[2])))



    # for i,node in enumerate(nodes):
        # print(i, node)

    nodes = np.array(nodes)



    distances = []
    best_dist = np.inf
    best_permutation = None

    for iter in range(N_ITER):
        perm = np.random.permutation(len(nodes))
        
        iter_dist = 0
        for i in range(len(nodes)):
            first = i
            second = (i+1)%len(nodes)
            dist = sqrt((nodes[perm[first]][0] - nodes[perm[second]][0]) ** 2 + (nodes[perm[first]][1] - nodes[perm[second]][1]) ** 2)
            iter_dist += dist
            # print(perm[first], ',', perm[second], ',', dist)
        if iter_dist < best_dist:
            best_dist = iter_dist
            best_permutation = perm
        distances.append(iter_dist)

    # groupings
    print(f"FILE: {str(file)}")
    for group_n in GROUPS_NS:
        print(f"\n\n=========GROUPING============\n group_n: {group_n}\n\n\n")
        indecies = np.arange(0,N_ITER,group_n)
        mins =  np.minimum.reduceat(np.array(distances), indecies)
        print(f"number of mins: {mins.shape}")

        # avg
        avg = np.mean(mins)
        print(f"\n AVG of mins:\n{avg}\n")

    # true min
    print("=========+MINIMUM=========")
    print(f"TRUE_MIN: {np.min(np.array(distances))}")

    path = nodes[best_permutation]

    # print(np.arange(1,len(path)+1,1)[best_permutation])

    plot_path(path, f"best_path_test_dist={best_dist}_f={str(file).split('/')[1]}_no_labels.png", np.arange(1,len(path)+1,1)[best_permutation])