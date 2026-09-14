#include "binary_tree_c.h"
#include <iostream>
#include <queue>

namespace BiTreeCStyle {

    // ==========================================
    // 1. Linked-storage Binary Tree Operations
    // ==========================================

    BiTree CreateNode(ElemType val) {
        BiTree node = new BiTNode();
        node->data = val;
        node->lchild = nullptr;
        node->rchild = nullptr;
        return node;
    }

    void DestroyTree(BiTree T) {
        if (T == nullptr) return;
        DestroyTree(T->lchild);
        DestroyTree(T->rchild);
        delete T;
    }

    void PreOrder(BiTree T) {
        if (T == nullptr) return;
        std::cout << T->data << " ";
        PreOrder(T->lchild);
        PreOrder(T->rchild);
    }

    void InOrder(BiTree T) {
        if (T == nullptr) return;
        InOrder(T->lchild);
        std::cout << T->data << " ";
        InOrder(T->rchild);
    }

    void PostOrder(BiTree T) {
        if (T == nullptr) return;
        PostOrder(T->lchild);
        PostOrder(T->rchild);
        std::cout << T->data << " ";
    }

    void LevelOrder(BiTree T) {
        if (T == nullptr) return;
        std::queue<BiTree> q;
        q.push(T);
        while (!q.empty()) {
            BiTree node = q.front();
            q.pop();
            std::cout << node->data << " ";
            if (node->lchild != nullptr) q.push(node->lchild);
            if (node->rchild != nullptr) q.push(node->rchild);
        }
    }

    int GetDepth(BiTree T) {
        if (T == nullptr) return 0;
        int leftDepth = GetDepth(T->lchild);
        int rightDepth = GetDepth(T->rchild);
        return std::max(leftDepth, rightDepth) + 1;
    }

    int GetNodeCount(BiTree T) {
        if (T == nullptr) return 0;
        return GetNodeCount(T->lchild) + GetNodeCount(T->rchild) + 1;
    }

    // ==========================================
    // 2. Thread Binary Tree Helper Functions
    // ==========================================

    ThreadNode *CreateThreadNode(ElemType val) {
        ThreadNode *node = new ThreadNode();
        node->data = val;
        node->lchild = nullptr;
        node->rchild = nullptr;
        node->ltag = 0;
        node->rtag = 0;
        return node;
    }

    void DestroyThreadTree(ThreadTree T) {
        if (T == nullptr) return;
        if (T->ltag == 0) DestroyThreadTree(T->lchild);
        if (T->rtag == 0) DestroyThreadTree(T->rchild);
        delete T;
    }

    // ==========================================
    // 3. In-order Thread Binary Tree Operations
    // ==========================================

    void Inthread(ThreadTree &p, ThreadTree &pre) {
        if (p == nullptr) return;

        Inthread(p->lchild, pre);

        if (p->lchild == nullptr) {
            p->lchild = pre;
            p->ltag = 1;
        }
        if (pre != nullptr && pre->rchild == nullptr) {
            pre->rchild = p;
            pre->rtag = 1;
        }
        pre = p;

        Inthread(p->rchild, pre);
    }

    void CreateInThread(ThreadTree T) {
        ThreadTree pre = nullptr;
        if (T != nullptr) {
            Inthread(T, pre);
            if (pre != nullptr && pre->rchild == nullptr) {
                pre->rtag = 1;
            }
        }
    }

    ThreadNode *Firstnode(ThreadNode *p) {
        while (p != nullptr && p->ltag == 0) {
            p = p->lchild;
        }
        return p;
    }

    ThreadNode *Lastnode(ThreadNode *p) {
        while (p != nullptr && p->rtag == 0) {
            p = p->rchild;
        }
        return p;
    }

    ThreadNode *Nextnode(ThreadNode *p) {
        if (p == nullptr) return nullptr;
        if (p->rtag == 1) return p->rchild;
        return Firstnode(p->rchild);
    }

    ThreadNode *Prenode(ThreadNode *p) {
        if (p == nullptr) return nullptr;
        if (p->ltag == 1) return p->lchild;
        return Lastnode(p->lchild);
    }

    void Inorder(ThreadNode *T) {
        for (ThreadNode *p = Firstnode(T); p != nullptr; p = Nextnode(p)) {
            std::cout << p->data << " ";
        }
    }

    void InorderRev(ThreadNode *T) {
        for (ThreadNode *p = Lastnode(T); p != nullptr; p = Prenode(p)) {
            std::cout << p->data << " ";
        }
    }

    // ==========================================
    // 4. Pre-order Thread Binary Tree Operations
    // ==========================================

    void PreThread(ThreadTree &p, ThreadTree &pre) {
        if (p == nullptr) return;

        if (p->lchild == nullptr) {
            p->lchild = pre;
            p->ltag = 1;
        }
        if (pre != nullptr && pre->rchild == nullptr) {
            pre->rchild = p;
            pre->rtag = 1;
        }
        pre = p;

        if (p->ltag == 0) PreThread(p->lchild, pre);
        if (p->rtag == 0) PreThread(p->rchild, pre);
    }

    void CreatePreThread(ThreadTree T) {
        ThreadTree pre = nullptr;
        if (T != nullptr) {
            PreThread(T, pre);
            if (pre != nullptr && pre->rchild == nullptr) {
                pre->rtag = 1;
            }
        }
    }

    ThreadNode *Nextnode_Pre(ThreadNode *p) {
        if (p == nullptr) return nullptr;
        if (p->rtag == 1) return p->rchild;
        if (p->ltag == 0) return p->lchild;
        return p->rchild;
    }

    void Preorder(ThreadNode *T) {
        for (ThreadNode *p = T; p != nullptr; p = Nextnode_Pre(p)) {
            std::cout << p->data << " ";
        }
    }

    // ==========================================
    // 5. Post-order Thread Binary Tree Operations
    // ==========================================

    void PostThread(ThreadTree &p, ThreadTree &pre) {
        if (p == nullptr) return;

        PostThread(p->lchild, pre);
        PostThread(p->rchild, pre);

        if (p->lchild == nullptr) {
            p->lchild = pre;
            p->ltag = 1;
        }
        if (pre != nullptr && pre->rchild == nullptr) {
            pre->rchild = p;
            pre->rtag = 1;
        }
        pre = p;
    }

    void CreatePostThread(ThreadTree T) {
        ThreadTree pre = nullptr;
        if (T != nullptr) {
            PostThread(T, pre);
            if (pre != nullptr && pre->rchild == nullptr) {
                pre->rtag = 1;
            }
        }
    }

    ThreadNode *Prenode_Post(ThreadNode *p) {
        if (p == nullptr) return nullptr;
        if (p->ltag == 1) return p->lchild;
        if (p->rtag == 0) return p->rchild;
        return p->lchild;
    }

    void PostorderRev(ThreadNode *T) {
        for (ThreadNode *p = T; p != nullptr; p = Prenode_Post(p)) {
            std::cout << p->data << " ";
        }
    }

} // namespace BiTreeCStyle