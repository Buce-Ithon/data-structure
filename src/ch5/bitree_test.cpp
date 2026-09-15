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

/**
 * @brief Q9: Calculates the width of a non-empty binary tree (maximum number of nodes at any single level).
 * 
 * @param T Pointer to the root of the binary tree.
 * @return int The maximum width of the binary tree.
 */
int GetTreeWidth(BiTree T) {
    if (T == nullptr) {
        return 0;
    }

    std::queue<BiTree> q;
    q.push(T);
    int maxWidth = 0;

    while (!q.empty()) {
        int levelSize = static_cast<int>(q.size());
        maxWidth = std::max(maxWidth, levelSize);

        for (int i = 0; i < levelSize; ++i) {
            BiTree node = q.front();
            q.pop();

            if (node->lchild != nullptr) {
                q.push(node->lchild);
            }
            if (node->rchild != nullptr) {
                q.push(node->rchild);
            }
        }
    }

    return maxWidth;
}

/**
 * @brief Q10: Recursive helper to convert a pre-order sequence of a full binary tree into a post-order sequence.
 * 
 * @param pre Pre-order sequence array.
 * @param l1 Start index of pre-order range.
 * @param r1 End index of pre-order range.
 * @param post Target post-order sequence array.
 * @param l2 Start index of post-order range.
 * @param r2 End index of post-order range.
 */
void PreToPostFullHelper(const ElemType pre[], int l1, int r1, ElemType post[], int l2, int r2) {
    if (l1 > r1) {
        return;
    }

    // Root of current subtree goes to the end of post-order range
    post[r2] = pre[l1];

    int half = (r1 - l1) / 2;
    // Process left subtree
    PreToPostFullHelper(pre, l1 + 1, l1 + half, post, l2, l2 + half - 1);
    // Process right subtree
    PreToPostFullHelper(pre, l1 + half + 1, r1, post, l2 + half, r2 - 1);
}

/**
 * @brief Q10: Computes post-order sequence from pre-order sequence for a full binary tree.
 * 
 * @param pre Pre-order sequence array.
 * @param n Total number of nodes in the full binary tree.
 * @param post Array to hold the resulting post-order sequence.
 */
void PreToPostFull(const ElemType pre[], int n, ElemType post[]) {
    if (n <= 0) return;
    PreToPostFullHelper(pre, 0, n - 1, post, 0, n - 1);
}

/**
 * @brief Q11: Recursive helper to traverse tree and link leaf nodes into a singly linked list.
 * 
 * @param T Pointer to current node.
 * @param head Reference to head pointer of the leaf list.
 * @param tail Reference to tail pointer of the leaf list.
 */
void LinkLeafNodesHelper(BiTree T, BiTree &head, BiTree &tail) {
    if (T == nullptr) {
        return;
    }

    // Check if node is a leaf node
    if (T->lchild == nullptr && T->rchild == nullptr) {
        if (head == nullptr) {
            head = T;
            tail = T;
        } else {
            tail->rchild = T; // Store link in right child field
            tail = T;
        }
        return;
    }

    LinkLeafNodesHelper(T->lchild, head, tail);
    LinkLeafNodesHelper(T->rchild, head, tail);
}

/**
 * @brief Q11: Links all leaf nodes of a binary tree from left to right into a singly linked list using rchild pointers.
 * 
 * @param T Pointer to the root of the binary tree.
 * @return BiTree Head pointer of the leaf node linked list.
 */
BiTree LinkLeafNodes(BiTree T) {
    BiTree head = nullptr;
    BiTree tail = nullptr;
    LinkLeafNodesHelper(T, head, tail);
    
    if (tail != nullptr) {
        tail->rchild = nullptr; // Terminate linked list
    }
    return head;
}

/**
 * @brief Q12: Checks whether two binary trees T1 and T2 are structurally similar.
 * 
 * @param T1 Pointer to root of the first binary tree.
 * @param T2 Pointer to root of the second binary tree.
 * @return true If T1 and T2 are structurally similar.
 * @return false Otherwise.
 */
bool IsSimilar(BiTree T1, BiTree T2) {
    if (T1 == nullptr && T2 == nullptr) {
        return true;
    }
    if (T1 == nullptr || T2 == nullptr) {
        return false;
    }
    return IsSimilar(T1->lchild, T2->lchild) && IsSimilar(T1->rchild, T2->rchild);
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
    
    // Application Q9
    std::cout << "===== Application Q9 =====" << std::endl;
    /*
     * Tree Q9 Structure:
     *            1           (Level 1: 1 node)
     *          /   \
     *         2     3         (Level 2: 2 nodes)
     *        / \     \
     *       4   5     6       (Level 3: 3 nodes - Max Width)
     *            \
     *             7          (Level 4: 1 node)
     */
    BiTree tree9 = CreateNode(1);
    tree9->lchild = CreateNode(2);
    tree9->rchild = CreateNode(3);
    tree9->lchild->lchild = CreateNode(4);
    tree9->lchild->rchild = CreateNode(5);
    tree9->rchild->rchild = CreateNode(6);
    tree9->lchild->rchild->rchild = CreateNode(7);

    std::cout << "Width of tree: " << GetTreeWidth(tree9) << std::endl;
    DestroyTree(tree9);

    // Application Q10
    std::cout << "===== Application Q10 =====" << std::endl;
    /*
     * Full Binary Tree (7 nodes):
     *            1
     *          /   \
     *         2     3
     *        / \   / \
     *       4   5 6   7
     * 
     * Pre-Order:  1, 2, 4, 5, 3, 6, 7
     * Post-Order: 4, 5, 2, 6, 7, 3, 1
     */
    ElemType pre[] = {1, 2, 4, 5, 3, 6, 7};
    int n = sizeof(pre) / sizeof(pre[0]);
    ElemType post[7];

    PreToPostFull(pre, n, post);

    std::cout << "Pre-Order:  ";
    for (int i = 0; i < n; ++i) std::cout << pre[i] << " ";
    std::cout << std::endl;

    std::cout << "Post-Order: ";
    for (int i = 0; i < n; ++i) std::cout << post[i] << " ";
    std::cout << std::endl;

    // Application Q11
    std::cout << "===== Application Q11 =====" << std::endl;
    /*
     * Tree Q11 Structure:
     *            10
     *          /    \
     *        20      30
     *       /  \       \
     *     40    50      60
     * Leaves from left to right: 40, 50, 60
     */
    BiTree tree11 = CreateNode(10);
    tree11->lchild = CreateNode(20);
    tree11->rchild = CreateNode(30);
    tree11->lchild->lchild = CreateNode(40);
    tree11->lchild->rchild = CreateNode(50);
    tree11->rchild->rchild = CreateNode(60);

    BiTree head = LinkLeafNodes(tree11);

    std::cout << "Leaf nodes linked list: ";
    for (BiTree p = head; p != nullptr; p = p->rchild) {
        std::cout << p->data << " ";
    }
    std::cout << std::endl;

    // Clean up nodes for tree11
    delete tree11->lchild->lchild;
    delete tree11->lchild->rchild;
    delete tree11->lchild;
    delete tree11->rchild->rchild;
    delete tree11->rchild;
    delete tree11;

    // Application Q12
    std::cout << "===== Application Q12 =====" << std::endl;
    /*
     * Tree Q12-A:      Tree Q12-B (Similar):     Tree Q12-C (Not Similar):
     *      1                    10                       100
     *     / \                  /  \                     /
     *    2   3                20  30                   200
     *   /                    /                        /
     *  4                    40                       300
     */
    BiTree tree12A = CreateNode(1);
    tree12A->lchild = CreateNode(2);
    tree12A->rchild = CreateNode(3);
    tree12A->lchild->lchild = CreateNode(4);

    BiTree tree12B = CreateNode(10);
    tree12B->lchild = CreateNode(20);
    tree12B->rchild = CreateNode(30);
    tree12B->lchild->lchild = CreateNode(40);

    BiTree tree12C = CreateNode(100);
    tree12C->lchild = CreateNode(200);
    tree12C->lchild->lchild = CreateNode(300);

    std::cout << "Are Tree 12A and Tree 12B Similar? " << (IsSimilar(tree12A, tree12B) ? "Yes" : "No") << std::endl;
    std::cout << "Are Tree 12A and Tree 12C Similar? " << (IsSimilar(tree12A, tree12C) ? "Yes" : "No") << std::endl;
    std::cout << "Are Tree 12B and Tree 12C Similar? " << (IsSimilar(tree12B, tree12C) ? "Yes" : "No") << std::endl;

    DestroyTree(tree12A);
    DestroyTree(tree12B);
    DestroyTree(tree12C);

    return 0;
}