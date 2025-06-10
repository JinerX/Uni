#if !defined(GRAPH_HPP)
#define GRAPH_HPP

#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <queue>
#include <algorithm>
#include <numeric>

struct Edge {
    int from, to;
    double weight;
    
    Edge(int f, int t, double w) : from(f), to(t), weight(w) {}
    
    bool operator>(const Edge& other) const {
        return weight > other.weight;
    }
};

class CompleteGraph {
private:
    int n;
    std::vector<std::vector<double>> adjacencyMatrix;

public:
    CompleteGraph(int vertices) : n(vertices) {
        adjacencyMatrix.resize(n, std::vector<double>(n, 0.0));
        generateRandomWeights();
    }

    void generateRandomWeights() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        
        // Gen matrix
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                double weight = dis(gen);
                adjacencyMatrix[i][j] = weight;
                adjacencyMatrix[j][i] = weight;
            }
        }
    }

    void printGraph() const {
        std::cout << "Graf pełny o " << n << " wierzchołkach:\n";
        std::cout << "Macierz sąsiedztwa (wagi krawędzi):\n\n";
        
        std::cout << "\n";
        for (int i = 0; i < n; i++) {
            std::cout << std::setw(8) << i;
        }
        std::cout << "\n";

        for (int i = 0; i < n; i++) {
            std::cout << std::setw(3) << i << ": ";
            for (int j = 0; j < n; j++) {
                if (i == j) {
                    std::cout << std::setw(8) << "0.000";
                } else {
                    std::cout << std::setw(8) << std::fixed << std::setprecision(3) 
                              << adjacencyMatrix[i][j];
                }
            }
            std::cout << "\n";
        }
    }

    void printEdgeList() const {
        std::cout << "\nLista krawędzi:\n";
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                std::cout << "Krawędź " << i << ", " << j << ": waga = " 
                          << std::fixed << std::setprecision(6) 
                          << adjacencyMatrix[i][j] << "\n";
            }
        }
    }

    // Prim's algorithm
    std::vector<Edge> primMST() const {
        std::vector<Edge> mst;
        std::vector<bool> inMST(n, false);
        std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> pq;
        
        inMST[0] = true;
        
        // add edges to pq
        for (int i = 1; i < n; i++) {
            pq.push(Edge(0, i, adjacencyMatrix[0][i]));
        }
        
        while (!pq.empty() && mst.size() < n - 1) {
            Edge minEdge = pq.top();
            pq.pop();
            
            if (inMST[minEdge.to]) {
                continue;
            }
            
            mst.push_back(minEdge);
            inMST[minEdge.to] = true;
            
            for (int i = 0; i < n; i++) {
                if (!inMST[i]) {
                    pq.push(Edge(minEdge.to, i, adjacencyMatrix[minEdge.to][i]));
                }
            }
        }
        
        return mst;
    }

    class UnionFind {
    private:
        std::vector<int> parent, rank;
        
    public:
        UnionFind(int n) : parent(n), rank(n, 0) {
            std::iota(parent.begin(), parent.end(), 0);
        }
        
        int find(int x) {
            if (parent[x] != x) {
                parent[x] = find(parent[x]);
            }
            return parent[x];
        }
        
        bool unite(int x, int y) {
            int px = find(x), py = find(y);
            if (px == py) return false;
            
            if (rank[px] < rank[py]) {
                parent[px] = py;
            } else if (rank[px] > rank[py]) {
                parent[py] = px;
            } else {
                parent[py] = px;
                rank[px]++;
            }
            return true;
        }
    };

    // Kruskal's algorithm
    std::vector<Edge> kruskalMST() const {
        std::vector<Edge> edges;
        std::vector<Edge> mst;
        
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                edges.push_back(Edge(i, j, adjacencyMatrix[i][j]));
            }
        }
        
        std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
            return a.weight < b.weight;
        });
        
        UnionFind uf(n);
        
        for (const Edge& edge : edges) {
            if (uf.unite(edge.from, edge.to)) {
                mst.push_back(edge);
                if (mst.size() == n - 1) break;
            }
        }
        
        return mst;
    }

    void printMST(const std::vector<Edge>& mst, const std::string& algorithmName) const {
        std::cout << "\n=== " << algorithmName << " - Minimal spanning tree ===\n";
        std::cout << "Edges in MST:\n";
        
        double totalWeight = 0.0;
        for (const Edge& edge : mst) {
            std::cout << "(" << edge.from << ", " << edge.to << ") - weught: " 
                      << std::fixed << std::setprecision(6) << edge.weight << "\n";
            totalWeight += edge.weight;
        }
        
        std::cout << "\nMST weight: " << std::fixed << std::setprecision(6) 
                  << totalWeight << "\n";
        std::cout << "Num edges MST: " << mst.size() << "\n";
    }

    double getWeight(int i, int j) const {
        if (i >= 0 && i < n && j >= 0 && j < n) {
            return adjacencyMatrix[i][j];
        }
        return -1;
    }

    int getVertexCount() const {
        return n;
    }

    int getEdgeCount() const {
        return (n * (n - 1)) / 2;
    }
};

#endif // GRAPH_HPP