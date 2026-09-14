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

/**
 * @brief Q2: Determines whether a given binary tree is a complete binary tree (CBT).
 * 
 * @details Uses level-order traversal (BFS). If a null node is encountered, 
 *          no subsequent non-null nodes should appear in the traversal.
 * 
 * @param T Pointer to the root of the binary tree.
 * @return true If the tree is a complete binary tree.
 * @return false Otherwise.
 */
bool IsCompleteBinaryTree(BiTree T) {
    if (T == nullptr) {
        return true;
    }

    std::queue<BiTree> q;
    q.push(T);
    bool seenNull = false;

    while (!q.empty()) {
        BiTree curr = q.front();
        q.pop();

        if (curr == nullptr) {
            seenNull = true;
        } else {
            // If a non-null node appears after a null node, it is not a CBT
            if (seenNull) {
                return false;
            }
            q.push(curr->lchild);
            q.push(curr->rchild);
        }
    }

    return true;
}

/**
 * @brief Q3: Counts the number of double-branch nodes (nodes with both left and right children) in a binary tree.
 * 
 * @param T Pointer to the root of the binary tree.
 * @return int The total number of double-branch nodes.
 */
int CountDoubleBranchNodes(BiTree T) {
    if (T == nullptr) {
        return 0;
    }

    int countSelf = (T->lchild != nullptr && T->rchild != nullptr) ? 1 : 0;
    return countSelf + CountDoubleBranchNodes(T->lchild) + CountDoubleBranchNodes(T->rchild);
}

/**
 * @brief Q4: Swaps the left and right subtrees of all nodes in a binary tree recursively.
 * 
 * @param T Pointer to the root of the binary tree.
 */
void SwapSubtrees(BiTree T) {
    if (T == nullptr) {
        return;
    }

    // Swap the left and right child pointers
    std::swap(T->lchild, T->rchild);

    // Recursively swap for subtrees
    SwapSubtrees(T->lchild);
    SwapSubtrees(T->rchild);
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
    
    // Application Q2
    std::cout << "===== Application Q2 =====" << std::endl;
    /*
     * Tree Q2-A (Complete Binary Tree):
     *        1
     *      /   \
     *     2     3
     *    / \   /
     *   4   5 6
     * 
     * Tree Q2-B (Incomplete Binary Tree):
     *        1
     *      /   \
     *     2     3
     *      \
     *       4
     */
    BiTree tree2A = CreateNode(1);
    tree2A->lchild = CreateNode(2);
    tree2A->rchild = CreateNode(3);
    tree2A->lchild->lchild = CreateNode(4);
    tree2A->lchild->rchild = CreateNode(5);
    tree2A->rchild->lchild = CreateNode(6);

    std::cout << "Is Tree 2A Complete? " << (IsCompleteBinaryTree(tree2A) ? "Yes" : "No") << std::endl;
    DestroyTree(tree2A);

    BiTree tree2B = CreateNode(1);
    tree2B->lchild = CreateNode(2);
    tree2B->rchild = CreateNode(3);
    tree2B->lchild->rchild = CreateNode(4);

    std::cout << "Is Tree 2B Complete? " << (IsCompleteBinaryTree(tree2B) ? "Yes" : "No") << std::endl;
    DestroyTree(tree2B);

    // Application Q3
    std::cout << "===== Application Q3 =====" << std::endl;
    /*
     * Tree Q3 Structure:
     *            10
     *          /    \
     *        20      30
     *       /  \    /  \
     *     40   50  60  70
     *     
     * Double-branch nodes: 10, 20, 30 (Total: 3)
     */
    BiTree tree3 = CreateNode(10);
    tree3->lchild = CreateNode(20);
    tree3->rchild = CreateNode(30);
    tree3->lchild->lchild = CreateNode(40);
    tree3->lchild->rchild = CreateNode(50);
    tree3->rchild->lchild = CreateNode(60);
    tree3->rchild->rchild = CreateNode(70);

    std::cout << "Double-branch node count: " << CountDoubleBranchNodes(tree3) << std::endl;

    DestroyTree(tree3);

    // Application Q4
    std::cout << "===== Application Q4 =====" << std::endl;
    /*
     * Tree Q4 Structure:
     *          100
     *         /   \
     *       200   300
     *       /       \
     *     400       500
     */
    BiTree tree4 = CreateNode(100);
    tree4->lchild = CreateNode(200);
    tree4->rchild = CreateNode(300);
    tree4->lchild->lchild = CreateNode(400);
    tree4->rchild->rchild = CreateNode(500);

    std::cout << "Original In-Order Traversal: ";
    InOrder(tree4);
    std::cout << std::endl;

    SwapSubtrees(tree4);

    std::cout << "In-Order Traversal after Swapping: ";
    InOrder(tree4);
    std::cout << std::endl;

    DestroyTree(tree4);

    return 0;
}