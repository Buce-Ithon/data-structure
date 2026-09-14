#ifndef BINARY_TREE_C_H
#define BINARY_TREE_C_H

namespace BiTreeCStyle {
    using ElemType = int;

    // Linked-storage Binary Tree
    typedef struct BiTNode {
        ElemType data;
        struct BiTNode *lchild, *rchild;
    } BiTNode, *BiTree;
    
    // Helper functions for memory management
    BiTree CreateNode(ElemType val);
    void DestroyTree(BiTree T);

    // Traverse operations
    void PreOrder(BiTree T);
    void InOrder(BiTree T);
    void PostOrder(BiTree T);
    void LevelOrder(BiTree T);
    
    // Helper utility functions
    int GetDepth(BiTree T);
    int GetNodeCount(BiTree T);

    // (Linked-storage) Thread Binary Tree
    // Tag definitions:
    // tag == 0: points to child node
    // tag == 1: points to thread (predecessor / successor)
    typedef struct ThreadNode {
        ElemType data;
        struct ThreadNode *lchild, *rchild;
        int ltag, rtag;
    } ThreadNode, *ThreadTree;
    
    // Helper functions for Thread Tree memory management
    ThreadNode *CreateThreadNode(ElemType val);
    void DestroyThreadTree(ThreadTree T);

    // In-order Thread Binary Tree
    void Inthread(ThreadTree &p, ThreadTree &pre);
    void CreateInThread(ThreadTree T);
    ThreadNode *Firstnode(ThreadNode *p);
    ThreadNode *Lastnode(ThreadNode *p);
    ThreadNode *Prenode(ThreadNode *p);
    ThreadNode *Nextnode(ThreadNode *p);
    void Inorder(ThreadNode *T);
    void InorderRev(ThreadNode *T);

    // Pre-order Thread Binary Tree
    void PreThread(ThreadTree &p, ThreadTree &pre);
    void CreatePreThread(ThreadTree T);
    ThreadNode *Nextnode_Pre(ThreadNode *p);
    void Preorder(ThreadNode *T);

    // Post-order Thread Binary Tree
    // Note: In a standard binary linked list (without parent pointers),
    // finding the post-order successor for nodes with rtag == 0 is impossible without additional context/stack.
    void PostThread(ThreadTree &p, ThreadTree &pre);
    void CreatePostThread(ThreadTree T);
    ThreadNode *Prenode_Post(ThreadNode *p);
    void PostorderRev(ThreadNode *T);

} // namespace BiTreeCStyle

#endif // BINARY_TREE_C_H