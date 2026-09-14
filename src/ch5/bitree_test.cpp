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

/**
 * @brief Q5: Helper recursive function to locate the k-th node in pre-order traversal.
 * 
 * @param T Pointer to the root of the binary tree.
 * @param k Target index (1-based).
 * @param count Reference to current traversal step counter.
 * @param val Reference to store node data when k-th node is located.
 * @return true If the k-th node is found.
 * @return false Otherwise.
 */
bool FindKthPreOrderHelper(BiTree T, int k, int &count, ElemType &val) {
    if (T == nullptr) {
        return false;
    }
    count++;
    if (count == k) {
        val = T->data;
        return true;
    }
    if (FindKthPreOrderHelper(T->lchild, k, count, val)) {
        return true;
    }
    return FindKthPreOrderHelper(T->rchild, k, count, val);
}

/**
 * @brief Q5: Finds the value of the k-th node in the pre-order traversal sequence.
 * 
 * @param T Pointer to the root of the binary tree.
 * @param k Target index (1 <= k <= total node count).
 * @param val Reference to store the found node value.
 * @return true If the k-th node exists and value is stored in val.
 * @return false If k is out of range or tree is empty.
 */
bool GetKthPreOrder(BiTree T, int k, ElemType &val) {
    int count = 0;
    return FindKthPreOrderHelper(T, k, count, val);
}

/**
 * @brief Q6: Deletes subtrees rooted at nodes with value x and frees allocated memory.
 * 
 * @param T Reference to the root pointer of the binary tree/subtree.
 * @param x Target node value to match for deletion.
 */
void DeleteSubtreesByValue(BiTree &T, ElemType x) {
    if (T == nullptr) {
        return;
    }
    if (T->data == x) {
        DestroyTree(T);
        T = nullptr;
        return;
    }
    DeleteSubtreesByValue(T->lchild, x);
    DeleteSubtreesByValue(T->rchild, x);
}

/**
 * @brief Q7: Searches for a node with target value x and prints all its ancestors.
 * 
 * @param T Pointer to the root of the binary tree.
 * @param x Value of the target node.
 * @return true If node with value x is found within subtree rooted at T.
 * @return false Otherwise.
 */
bool PrintAncestors(BiTree T, ElemType x) {
    if (T == nullptr) {
        return false;
    }
    if (T->data == x) {
        return true;
    }
    if (PrintAncestors(T->lchild, x) || PrintAncestors(T->rchild, x)) {
        std::cout << T->data << " ";
        return true;
    }
    return false;
}

/**
 * @brief Q8: Finds the Lowest Common Ancestor (LCA) node of two target nodes p and q.
 * 
 * @param T Pointer to the root of the binary tree.
 * @param p Pointer to the first target node.
 * @param q Pointer to the second target node.
 * @return BiTree Pointer to the lowest common ancestor node, or nullptr if not found.
 */
BiTree FindLCA(BiTree T, BiTree p, BiTree q) {
    if (T == nullptr || T == p || T == q) {
        return T;
    }
    BiTree leftLCA = FindLCA(T->lchild, p, q);
    BiTree rightLCA = FindLCA(T->rchild, p, q);

    if (leftLCA != nullptr && rightLCA != nullptr) {
        return T;
    }
    return (leftLCA != nullptr) ? leftLCA : rightLCA;
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

    // Application Q5
    std::cout << "===== Application Q5 =====" << std::endl;
    /*
     * Tree Q5 Structure:
     *            1
     *          /   \
     *         2     3
     *        / \
     *       4   5
     * Pre-Order sequence: 1, 2, 4, 5, 3
     */
    BiTree tree5 = CreateNode(1);
    tree5->lchild = CreateNode(2);
    tree5->rchild = CreateNode(3);
    tree5->lchild->lchild = CreateNode(4);
    tree5->lchild->rchild = CreateNode(5);

    int targetK = 3;
    ElemType kthVal;
    if (GetKthPreOrder(tree5, targetK, kthVal)) {
        std::cout << "The " << targetK << "-th node in Pre-Order traversal is: " << kthVal << std::endl;
    } else {
        std::cout << "Invalid k = " << targetK << std::endl;
    }
    DestroyTree(tree5);

    // Application Q6
    std::cout << "===== Application Q6 =====" << std::endl;
    /*
     * Tree Q6 Structure:
     *            1
     *          /   \
     *         2     3
     *        / \     \
     *       4   5     6
     * Target x = 2 (Delete subtree rooted at node 2)
     */
    BiTree tree6 = CreateNode(1);
    tree6->lchild = CreateNode(2);
    tree6->rchild = CreateNode(3);
    tree6->lchild->lchild = CreateNode(4);
    tree6->lchild->rchild = CreateNode(5);
    tree6->rchild->rchild = CreateNode(6);

    std::cout << "Pre-Order before deletion of x = 2: ";
    PreOrder(tree6);
    std::cout << std::endl;

    DeleteSubtreesByValue(tree6, 2);

    std::cout << "Pre-Order after deletion: ";
    PreOrder(tree6);
    std::cout << std::endl;
    DestroyTree(tree6);

    // Application Q7
    std::cout << "===== Application Q7 =====" << std::endl;
    /*
     * Tree Q7 Structure:
     *            10
     *          /    \
     *        20      30
     *       /  \
     *     40    50
     * Target x = 50 -> Ancestors expected: 20, 10
     */
    BiTree tree7 = CreateNode(10);
    tree7->lchild = CreateNode(20);
    tree7->rchild = CreateNode(30);
    tree7->lchild->lchild = CreateNode(40);
    tree7->lchild->rchild = CreateNode(50);

    ElemType targetX = 50;
    std::cout << "Ancestors of node " << targetX << ": ";
    if (!PrintAncestors(tree7, targetX)) {
        std::cout << "Node not found.";
    }
    std::cout << std::endl;
    DestroyTree(tree7);

    // Application Q8
    std::cout << "===== Application Q8 =====" << std::endl;
    /*
     * Tree Q8 Structure:
     *            1
     *          /   \
     *         2     3
     *        / \
     *       4   5
     * Nodes: p = 4, q = 5 -> LCA expected: 2
     * Nodes: p = 4, q = 3 -> LCA expected: 1
     */
    BiTree tree8 = CreateNode(1);
    tree8->lchild = CreateNode(2);
    tree8->rchild = CreateNode(3);
    tree8->lchild->lchild = CreateNode(4);
    tree8->lchild->rchild = CreateNode(5);

    BiTree pNode = tree8->lchild->lchild; // Node 4
    BiTree qNode = tree8->lchild->rchild; // Node 5

    BiTree lca1 = FindLCA(tree8, pNode, qNode);
    if (lca1 != nullptr) {
        std::cout << "LCA of " << pNode->data << " and " << qNode->data << " is: " << lca1->data << std::endl;
    }

    BiTree qNode2 = tree8->rchild; // Node 3
    BiTree lca2 = FindLCA(tree8, pNode, qNode2);
    if (lca2 != nullptr) {
        std::cout << "LCA of " << pNode->data << " and " << qNode2->data << " is: " << lca2->data << std::endl;
    }

    DestroyTree(tree8);
    return 0;
}