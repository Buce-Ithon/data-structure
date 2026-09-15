#include <iostream>
#include "binary_tree_c.h"

using namespace BiTreeCStyle;

typedef struct node {
    ElemType data;
    struct node *fch, *nsib;
} *Tree, *CSTree;

/**
 * @brief Helper function to create a new node.
 */
Tree createNode(ElemType data) {
    Tree newNode = new node();
    newNode->data = data;
    newNode->fch = nullptr;
    newNode->nsib = nullptr;
    return newNode;
}

/**
 * @brief Helper function to free allocated tree nodes recursively.
 */
void freeTree(Tree T) {
    if (T == nullptr) return;
    freeTree(T->fch);
    freeTree(T->nsib);
    delete T;
}

/**
 * @brief Q1: Find the number of leaf nodes in a forest stored using the child-sibling representation.
 * 
 * @param T Pointer to the root of the child-sibling binary tree.
 * @return int The number of leaf node in the forest.
 */
int Leaves(Tree T) {
    if (T==nullptr) {
        return 0;
    }
    if (T->fch==nullptr) {
        return 1 + Leaves(T->nsib);
    } else {
        return Leaves(T->fch) + Leaves(T->nsib);
    }
}

/**
 * @brief Q2: Design an algorithm to calculate the depth of a tree using the child-sibling linked list as the storage structure.
 * 
 * @param BT Pointer to the root of the child-sibling binary tree.
 * @return int The height of the tree (0 if the tree is empty).
 */
int Height(CSTree BT) {
    int hc, hs;
    if (BT==nullptr) {
        return 0;
    } else {
        hc = Height(BT->fch);
        hs = Height(BT->nsib);
        if (hc + 1 > hs) {
            return hc + 1;
        } else {
            return hs;
        }
    }
}

int main() {
    /*
     * Constructing the following forest structure:
     * 
     * Forest Structure:
     *     Tree 1:        Tree 2:
     *        A              E
     *       / \            /
     *      B   C          F
     *     /
     *    D
     * 
     * Child-Sibling Representation (Binary Tree Structure):
     *        A --- nsib ---> E
     *       /               /
     *      B --- nsib -> C F
     *     /
     *    D
     */

    // Tree 1 Nodes
    Tree nodeA = createNode('A');
    Tree nodeB = createNode('B');
    Tree nodeC = createNode('C');
    Tree nodeD = createNode('D');

    // Tree 2 Nodes
    Tree nodeE = createNode('E');
    Tree nodeF = createNode('F');

    // Linking Child-Sibling relations
    nodeA->fch = nodeB;
    nodeA->nsib = nodeE;

    nodeB->fch = nodeD;
    nodeB->nsib = nodeC;

    nodeE->fch = nodeF;

    // Application Q1
    std::cout << "===== Application Q1 =====" << std::endl;
    int leafCount = Leaves(nodeA);
    std::cout << "Number of leaf nodes in forest: " << leafCount << " (Expected: 3)" << std::endl;

    // Application Q2
    std::cout << "===== Application Q2 =====" << std::endl;
    int treeHeight = Height(nodeA);
    std::cout << "Height of tree/forest: " << treeHeight << " (Expected: 3)" << std::endl;

    // Clean up allocated memory
    freeTree(nodeA);

    return 0;
}