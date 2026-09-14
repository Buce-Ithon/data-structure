#include <iostream>
#include "binary_tree_c.h"
#include <queue>

using namespace BiTreeCStyle;

/**
 * @brief Q1: Calculates the height of a binary tree using a recursive algorithm.
 * 
 * @param T Pointer to the root of the binary tree.
 * @return int The height of the binary tree (0 if the tree is empty).
 */
int GetHeightRecursive(BiTree T) {
    if (T == nullptr) {
        return 0;
    }
    int leftHeight = GetHeightRecursive(T->lchild);
    int rightHeight = GetHeightRecursive(T->rchild);
    return std::max(leftHeight, rightHeight) + 1;
}

/**
 * @brief Q1: Calculates the height of a binary tree using a non-recursive (level-order BFS) algorithm.
 * 
 * @param T Pointer to the root of the binary tree.
 * @return int The height of the binary tree (0 if the tree is empty).
 */
int GetHeightNonRecursive(BiTree T) {
    if (T == nullptr) {
        return 0;
    }

    std::queue<BiTree> q;
    q.push(T);
    int height = 0;

    while (!q.empty()) {
        size_t levelSize = q.size(); // Number of nodes at current level

        for (size_t i = 0; i < levelSize; ++i) {
            BiTree node = q.front();
            q.pop();

            if (node->lchild != nullptr) {
                q.push(node->lchild);
            }
            if (node->rchild != nullptr) {
                q.push(node->rchild);
            }
        }
        height++; // Increment height after processing all nodes of the current level
    }

    return height;
}

int main() {
    // Application Q1
    std::cout << "===== Application Q1 =====" << std::endl;
    /*
     * Constructing a sample binary tree:
     *
     *            1
     *          /   \
     *         2     3
     *        / \     \
     *       4   5     6
     *          /
     *         7
     *
     * Expected Height: 4 (Path: 1 -> 2 -> 5 -> 7)
     */

    BiTree root = CreateNode(1);
    root->lchild = CreateNode(2);
    root->rchild = CreateNode(3);

    root->lchild->lchild = CreateNode(4);
    root->lchild->rchild = CreateNode(5);
    root->rchild->rchild = CreateNode(6);

    root->lchild->rchild->lchild = CreateNode(7);

    // Test Recursive Height Algorithm
    int recHeight = GetHeightRecursive(root);
    std::cout << "Height (Recursive): " << recHeight << std::endl;

    // Test Non-Recursive Height Algorithm
    int nonRecHeight = GetHeightNonRecursive(root);
    std::cout << "Height (Non-Recursive): " << nonRecHeight << std::endl;

    // Clean up allocated memory
    DestroyTree(root);
    return 0;
}