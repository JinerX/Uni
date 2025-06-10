#include "BinomialHeap.hpp"

long long BinomialHeap::comparisonCount = 0;

BinomialNode* BinomialHeap::mergeTrees(BinomialNode* tree1, BinomialNode* tree2) {
    comparisonCount++;
    if (tree1->key > tree2->key) {
        std::swap(tree1, tree2);
    }
    
    tree2->parent = tree1;
    tree2->sibling = tree1->child;
    tree1->child = tree2;
    tree1->degree++;
    
    return tree1;
}

BinomialNode* BinomialHeap::mergeHeaps(BinomialNode* h1, BinomialNode* h2) {
    if (!h1) return h2;
    if (!h2) return h1;
    
    BinomialNode* newHead = nullptr;
    BinomialNode* tail = nullptr;
    
    while (h1 && h2) {
        BinomialNode* temp;
        if (h1->degree <= h2->degree) {
            temp = h1;
            h1 = h1->sibling;
        } else {
            temp = h2;
            h2 = h2->sibling;
        }
        
        if (!newHead) {
            newHead = tail = temp;
        } else {
            tail->sibling = temp;
            tail = temp;
        }
    }
    
    if (h1) {
        tail->sibling = h1;
    } else {
        tail->sibling = h2;
    }
    
    return newHead;
}

BinomialNode* BinomialHeap::unionHeaps(BinomialNode* h1, BinomialNode* h2) {
    BinomialNode* merged = mergeHeaps(h1, h2);
    if (!merged) return nullptr;
    
    BinomialNode* prev = nullptr;
    BinomialNode* curr = merged;
    BinomialNode* next = merged->sibling;
    
    while (next) {
        if (curr->degree != next->degree || 
            (next->sibling && next->sibling->degree == curr->degree)) {
            prev = curr;
            curr = next;
        } else {
            if (curr->key <= next->key) {
                curr->sibling = next->sibling;
                curr = mergeTrees(curr, next);
            } else {
                if (prev) {
                    prev->sibling = next;
                } else {
                    merged = next;
                }
                curr = mergeTrees(next, curr);
            }
        }
        next = curr->sibling;
    }
    
    return merged;
}

void BinomialHeap::printTree(BinomialNode* node, int depth) {
    if (!node) return;
    
    for (int i = 0; i < depth; i++) {
        std::cout << "  ";
    }
    std::cout << node->key << " (degree: " << node->degree << ")" << std::endl;
    
    printTree(node->child, depth + 1);
    printTree(node->sibling, depth);
}

void BinomialHeap::resetComparisonCount() {
    comparisonCount = 0;
}

long long BinomialHeap::getComparisonCount() {
    return comparisonCount;
}

void BinomialHeap::insert(int key) {
    BinomialNode* newNode = new BinomialNode(key);
    BinomialHeap tempHeap;
    tempHeap.head = newNode;
    
    head = unionHeaps(head, tempHeap.head);
    tempHeap.head = nullptr;
}

int BinomialHeap::extractMin() {
    if (!head) return INT_MAX;
    
    BinomialNode* minNode = head;
    BinomialNode* minPrev = nullptr;
    BinomialNode* prev = nullptr;
    BinomialNode* curr = head;
    
    while (curr) {
        comparisonCount++;
        if (curr->key < minNode->key) {
            minNode = curr;
            minPrev = prev;
        }
        prev = curr;
        curr = curr->sibling;
    }
    
    if (minPrev) {
        minPrev->sibling = minNode->sibling;
    } else {
        head = minNode->sibling;
    }
    
    BinomialNode* childHead = minNode->child;
    
    BinomialNode* reversedChildren = nullptr;
    while (childHead) {
        BinomialNode* next = childHead->sibling;
        childHead->sibling = reversedChildren;
        childHead->parent = nullptr;
        reversedChildren = childHead;
        childHead = next;
    }
    
    head = unionHeaps(head, reversedChildren);
    
    int minKey = minNode->key;
    delete minNode;
    return minKey;
}

void BinomialHeap::unionWith(BinomialHeap& other) {
    head = unionHeaps(head, other.head);
    other.head = nullptr;
}

bool BinomialHeap::isEmpty() {
    return head == nullptr;
}

int BinomialHeap::peekMin() {
    if (!head) return INT_MAX;
    
    BinomialNode* curr = head;
    int minKey = curr->key;
    
    while (curr) {
        if (curr->key < minKey) {
            minKey = curr->key;
        }
        curr = curr->sibling;
    }
    
    return minKey;
}

void BinomialHeap::print() {
    std::cout << "Binomial Heap:" << std::endl;
    if (!head) {
        std::cout << "Empty heap" << std::endl;
        return;
    }
    
    BinomialNode* curr = head;
    int treeNum = 0;
    while (curr) {
        std::cout << "Tree " << treeNum++ << ":" << std::endl;
        printTree(curr, 0);
        curr = curr->sibling;
    }
    std::cout << std::endl;
}

BinomialHeap::~BinomialHeap() {
    while (!isEmpty()) {
        extractMin();
    }
}