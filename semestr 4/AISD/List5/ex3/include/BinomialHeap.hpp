#if !defined(BINOMIAL_HEAP_HPP)
#define BINOMIAL_HEAP_HPP

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <climits>

class BinomialNode {
public:
    int key;
    int degree;
    BinomialNode* parent;
    BinomialNode* child;
    BinomialNode* sibling;
    
    BinomialNode(int k) : key(k), degree(0), parent(nullptr), child(nullptr), sibling(nullptr) {}
};

class BinomialHeap {
private:
    BinomialNode* head;
    static long long comparisonCount;
    
    BinomialNode* mergeTrees(BinomialNode* tree1, BinomialNode* tree2);
    BinomialNode* mergeHeaps(BinomialNode* h1, BinomialNode* h2);
    BinomialNode* unionHeaps(BinomialNode* h1, BinomialNode* h2);
    void printTree(BinomialNode* node, int depth = 0);
    
public:
    BinomialHeap() : head(nullptr) {}
    
    static void resetComparisonCount();
    static long long getComparisonCount();
    
    void insert(int key);
    int extractMin();
    void unionWith(BinomialHeap& other);
    bool isEmpty();
    int peekMin();
    void print();
    
    ~BinomialHeap();
};

#endif // BINOMIAL_HEAP_HPP