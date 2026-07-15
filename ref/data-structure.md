# Introduction

1. **数据**：所有能被输入到计算机且供计算机程序处理的客观事物的符号表示

    基本单位：**数据元素**；数据元素又可以细分为**数据项**

    具有相同性质的数据元素可以构成**数据对象**

2. 类比代数结构，**数据结构**是<u>数据元素+关系</u>的集合

    Data Structure = (D, S)

    依据关系可以分为4类基本数据结构：*集合*，*线性结构*，*树形结构*，*图状/网状结构*
    
    > 想一想，这4类基本数据结构中数据元素的关系，即它们的划分依据是什么？
    
3. **数据类型**是一类值和定义在其上的操作的总称

    按照是否可细分可以分为2类：*原子类型*，*结构类型*
    
    **抽象数据类型**是数据类型在范畴意义上的抽象，更注重数据类型的抽象数学特性而非具体实现
    
    Abstract Data Type = (D, S, P) P是对D的基本操作集合
    
    **多态数据类型**是其值的成分不确定的数据类型
    
4. **算法**是解决特定问题的策略实现

    特征：*有穷性*，*确定性*，*可行性*，*输入*，*输出*
    
    衡量：**时间复杂度** 和 **空间复杂度**
    
# 线性结构

**线性结构**Definition: 在数据元素的非空有限集中，（1）存在唯一的只有一个后继的“第一个”数据元素；（2）存在唯一的只有一个前驱的“最后一个”元素；（3）除前两者之外每个数据元素均只有一个前驱和一个后继

## 线性表

抽象数据类型
```
ADT List {
  数据对象：D = { a_i | a_i ∈ ElemSet, i = 1,2,...,n, n ≥ 0 }
  数据关系：R = { <a_{i-1}, a_i> | a_{i-1}, a_i ∈ D, i = 2,...,n }
  基本操作：
    InitList(&L)
      初始条件：无
      操作结果：构造一个空的线性表 L。
    DestroyList(&L)
      初始条件：线性表 L 已存在
      操作结果：销毁线性表 L。
    ClearList(&L)
      初始条件：线性表 L 已存在
      操作结果：将 L 重置为空表。
    ListEmpty(L)
      初始条件：线性表 L 已存在
      操作结果：若 L 为空表，则返回 true，否则返回 false。
    ListLength(L)
      初始条件：线性表 L 已存在
      操作结果：返回 L 中数据元素个数。
    GetElem(L, i, &e)
      初始条件：线性表 L 已存在，1 ≤ i ≤ ListLength(L)
      操作结果：用 e 返回 L 中第 i 个数据元素的值。
    LocateElem(L, e, compare())
      初始条件：线性表 L 已存在，compare() 是元素判定函数
      操作结果：返回 L 中第一个与 e 满足 compare() 的数据元素的位序。若不存在，则返回 0。
    PriorElem(L, cur_e, &pre_e)
      初始条件：线性表 L 已存在
      操作结果：若 cur_e 是 L 中的数据元素且不是第一个，则用 pre_e 返回其前驱；否则操作失败，pre_e 无定义。
    NextElem(L, cur_e, &next_e)
      初始条件：线性表 L 已存在
      操作结果：若 cur_e 是 L 中的数据元素且不是最后一个，则用 next_e 返回其后继；否则操作失败，next_e 无定义。
    ListInsert(&L, i, e)
      初始条件：线性表 L 已存在，1 ≤ i ≤ ListLength(L)+1
      操作结果：在 L 中第 i 个位置之前插入新的数据元素 e，L 的长度增加 1。
    ListDelete(&L, i, &e)
      初始条件：线性表 L 已存在且非空，1 ≤ i ≤ ListLength(L)
      操作结果：删除 L 的第 i 个数据元素，并用 e 返回其值，L 的长度减少 1。
    ListTraverse(L, visit())
      初始条件：线性表 L 已存在，visit() 是元素操作函数
      操作结果：依次对 L 的每个数据元素调用 visit()。一旦 visit() 失败，则操作失败。
} ADT List
```

> Miscellaneous Talk
> And peace unto you my friend. May your tide raise many boats.

存储方式：*顺序存储*和*链式存储*

*顺序存储*：每个存储单元（**结点**）仅存储当前位置的数据值（**数据域**），物理位置连续，初始化线性表、插入、删除等操作都可以通过简单的连续指针操作来实现

*链式存储*：每个存储单元（**结点**）需存储当前位置的数据值（**数据域**）和（直接）后继存储位置（**指针域**），物理位置任意，n个结点链结成一个**链表**，初始化线性表、插入、删除等操作需要同时操作数据域和指针域，合理利用空间

带头节点的线性链表类型实现（注意这还是线性表的一种，仍然遵循线性表的抽象数据类型，以下只是其C语言具体实现，顺序存储的线性表可以直接由线性表的抽象数据类型实现，而链表在具体实现上需要添加对指针域的手动更改等操作(这一部分在抽象数据类型中没有直接体现出来)，又由于链表比较重要，因此我们单独实现其类型）
```C
typedef struct LNode {// 节点类型
    ElemType              data;
    struct LNode          * next;
} * Link, * Position;

typedef struct{       // 链表类型
    Link head, tail;  // 分别指向线性链表中的头结点和最后一个结点
    int  len;         // 指示线性链表中元素的个数
} LinkList;

Status MakeNode( Link &p, ElemType e );
    // 分配由p指向的值为e的结点，并返回OK；若分配失败，则返回ERROR
void FreeNode( Link &p );
    // 释放p所指的结点

Satus InitList( LinkList &L );
    // 构造一个空的线性链表 L
Status DestroyList( LinkList &L );
    // 销毁一个线性链表 L
Status ClearList( LinkList &L );
    // 将线性链表 L 重置为空表，释放原列表的结点空间
Status InsFirst( Link h, Link s );
    // 已知 h 指向线性链表的头结点，将 s 所指结点插入在第一个结点之前
Status DelFirst( Link h, Link &q );
    // 已知 h 指向线性链表的头结点，删除链表中的第一个结点，返回为 q
Status Append( LinkList &L, Link s );
    // 将指针 s 所指的一串结点链接在线性链表 L 的最后一个结点
    // 之后，并改变链表 L 的尾指针指向新的尾结点
Status Remove( LinkList &L, Link &q );
    // 删除线性链表 L 中的尾结点，返回为 q，并改变链表 L 的尾指针指向新的尾结点
Status InsBefore( LinkList &L, Link &p, Link s );
    // 已知 p 指向线性链表 L 中的一个结点，将 s 所指结点插入到 p 所指结点之前
    // 并修改 s 所指结点和 s 之前所指结点的 next 指针
Status InsAfter( LinkList &L, Link &p, Link s );
    // 已知 p 指向线性链表 L 中的一个结点，将 s 所指结点插入到 p 所指结点之后
    // 并修改 p 所指结点和 s 所指结点的 next 指针
Status SetCurElem( Link &p, ElemType e );
    // 已知 p 指向线性链表中的一个结点，用 e 更新 p 所指数据元素的值
ElemType GetCrurElem( Link &p );
    // 已知 p 指向线性链表中的一个结点，返回 p 所指数据元素的值
Status IsListEmpty( LinkList L );
    // 若线性链表 L 为空表，则返回为 TRUE，反之返回为 FALSE
int ListLength( LinkList L );
    // 返回线性链表 L 中元素的个数
Position GetHead( LinkList L );
    // 返回线性链表 L 中头结点的位置
Position GetTail( LinkList L );
    // 返回线性链表 L 中尾结点的位置
Position PriorPos( LinkList L, Link p );
    // 已知 p 指向线性链表 L 中的一个结点，返回 p 所指结点的直接前驱
    // 若无直接前驱，则返回NULL
Position NextPos( LinkList L, Link p );
    // 已知 p 指向线性链表 L 中的一个结点，返回 p 所指结点的直接后继
    // 若无直接后继，则返回NULL
Status LocatePos( LinkList L, int i, Link &p );
    // 将 p 指示为线性链表中第 i 个结点的位置并返回 OK，若 i 不合法则返回 ERROR
Position LocateElem( LinkList L, ElemType e, Status (* compare)(ElemType, ElemTpye) );
    // 返回线性链表 L 中，第一个和 e 满足 compare() 判断关系的元素位置
    // 若不存在这样的关系，返回为 NULL
Status ListTraverse( LinkList L, Status (* visit)() );
    // 依次对线性链表中的每个元素调用 visit() 函数，一旦 visit() 失败，则返回 ERROR
```

> 注意：涉及一个结点的直接前驱的操作的时间复杂度往往都和表长成正比，上述类型的定义中有些方法的时间复杂度和表长成正比容易判断，有些则需要用到前面提到的这一注意点，你分别可以找一找

> 你能设计一个能够高效利用存储空间又能进行加法和乘法运算的一元多项式的抽象数据类型吗
>
> Tips: 基于前面学习的线性链表，将数据域设计成存储系数和指数的数组

## 栈

**栈**是限定仅在表尾（栈顶(top)）进行插入或删除操作的线性表

特征：后进先出

抽象数据结构
```
ADT Stack {
    数据对象：D = { a_i | a_i ∈ ElemSet, i = 1, 2, ..., n, n ≥ 0 }
    数据关系：R1 = { <a_{i-1}, a_i> | a_{i-1}, a_i ∈ D, i = 2, ..., n }
             约定a_n为栈顶，a_1为栈底

    基本操作：
        InitStack(&S)
            操作结果：构造一个空栈S。
        DestroyStack(&S)
            初始条件：栈S已存在。
            操作结果：栈S被销毁。
        ClearStack(&S)
            初始条件：栈S已存在。
            操作结果：将S清为空栈。
        StackEmpty(S)
            初始条件：栈S已存在。
            操作结果：若S为空栈，则返回TRUE，否则返回FALSE。
        StackLength(S)
            初始条件：栈S已存在。
            操作结果：返回S中元素个数，即栈的长度。
        GetTop(S, &e)
            初始条件：栈S已存在且非空。
            操作结果：用e返回S的栈顶元素。
        Push(&S, e) // 入栈
            初始条件：栈S已存在。
            操作结果：插入元素e为新的栈顶元素。
        Pop(&S, &e) // 出栈
            初始条件：栈S已存在且非空。
            操作结果：删除S的栈顶元素，并用e返回其值。
        StackTraverse(S, visit())
            初始条件：栈S已存在且非空。
            操作结果：从栈底到栈顶依次对S的每个元素调用函数visit()。
} ADT Stack
```

存储方式：*顺序存储*和*链式存储*

```C
// ===== ADT Stack 的顺序存储实现=====
#define STACK_INIT_SIZE 100   // 存储空间初始分配量
#define STACKINCREMENT 10     // 存储空间分配增量

typedef int SElemType;        // 元素类型，可根据需要修改

// -------- 栈的顺序存储结构 ----------
struct SqStack {
    SElemType *base;   // 栈底指针，构造前和销毁后为 nullptr
    SElemType *top;    // 栈顶指针，指向栈顶元素的下一个位置
    int stacksize;     // 当前已分配的存储容量（以元素为单位）
};

// -------- 基本操作函数声明 ----------
Status InitStack(SqStack &S);
Status DestroyStack(SqStack &S);
Status ClearStack(SqStack &S);
Status StackEmpty(SqStack S);
int StackLength(SqStack S);
Status GetTop(SqStack S, SElemType &e);
Status Push(SqStack &S, SElemType e);
Status Pop(SqStack &S, SElemType &e);
Status StackTraverse(SqStack S, Status (*visit)());

// -------- 基本操作实现 ----------
// 构造一个空栈 S
Status InitStack(SqStack &S) {
    S.base = (SElemType *)malloc(STACK_INIT_SIZE * sizeof(SElemType));
    if (!S.base) exit (OVERFLOW);   // 分配失败
    S.top = S.base;
    S.stacksize = STACK_INIT_SIZE;
    return OK;
}

// 销毁栈 S
Status DestroyStack(SqStack &S) {
    if (S.base) {
        free(S.base);
        S.base = NULL;
    }
    S.top = NULL;
    S.stacksize = 0;
    return OK;
}

// 将 S 清为空栈
Status ClearStack(SqStack &S) {
    if (!S.base) return ERROR;   // 栈未构造
    S.top = S.base;
    return OK;
}

// 判断栈 S 是否为空
Status StackEmpty(SqStack S) {
    return S.base == S.top;
}

// 返回栈 S 的长度（元素个数）
int StackLength(SqStack S) {
    return (int)(S.top - S.base);
}

// 获取栈顶元素，通过 e 返回
Status GetTop(SqStack S, SElemType &e) {
    if (S.base == S.top) return ERROR;   // 空栈
    e = *(S.top - 1);
    return OK;
}

// 插入元素 e 为新的栈顶元素
Status Push(SqStack &S, SElemType e) {
    if (!S.base) return ERROR;   // 栈未构造
    // 栈满，需追加存储空间
    if (S.top - S.base >= S.stacksize) {
        S.base = (SElemType *)realloc(S.base,
                    (S.stacksize + STACKINCRECEMENT) * sizeof(ElemType));
        if (!S.base) exit (OVERFLOW); // 存储分配失败
        S.top = S.base + S.stacksize;
        S.stcksize += STACKINCRECEMENT;
    }
    *S.top++ = e;
    return OK;
}

// 删除栈顶元素，并用 e 返回其值
Status Pop(SqStack &S, SElemType &e) {
    if (!S.base || S.base == S.top) return ERROR;   // 空栈
    e = *(--S.top);
    return OK;
}

// 从栈底到栈顶依次对每个元素调用 visit 函数
Status StackTraverse(SqStack S, Status (*visit)()) {
    if (!S.base) return ERROR;
    SElemType *p = S.base;
    while (p < S.top) {
        visit(*p);
        ++p;
    }
    return OK;
}
```

> 注释：为方便起见，栈的第一个结点往往是一个特殊的结点，其 next 指针指向的下一个节点才是栈底结点（即真正的栈的第一个节点），包括后面的队列也是如此，你能想一想这是为什么吗？（联系空栈的判断方法就很清楚了）

**栈的应用**：求解迷宫问题、计算表达式、函数调用（也包括递归函数）

## 队列

**队列**是限定在队尾(rear)进行插入、队头(front)进行删除的线性表

特点：先进先出

抽象数据类型
```
ADT Queue {
    数据对象：D = { a_i | a_i ∈ ElemType, i = 1..n, n ≥ 0 }
    数据关系：R1 = { <a_i, a_{i+1}> | a_i, a_{i+1} ∈ D, 1 ≤ i < n }
    基本操作：
        InitQueue(&Q)      // 构造空队列
        DestroyQueue(&Q)   // 销毁队列
        ClearQueue(&Q)     // 清空队列
        QueueEmpty(Q)      // 判空
        QueueLength(Q)     // 求长度
        GetHead(Q, &e)     // 取队头元素
        EnQueue(&Q, e)     // 入队
        DeQueue(&Q, &e)    // 出队
        QueueTraverse(Q, visit())  // 遍历队列
} ADT Queue
```

存储方式：*顺序存储*和*链式存储*（链队列）

```C
// ===== ADT Queue 的表示与实现 =====

// ----- 单链队列--队列的链式存储结构 -----
typedef struct QNode {
    ElemeType data;
    struct QNode * Next;
}QNode, * QueuePtr;
typedef struct {
    QueuePtr front; // 队头指针
    QueuePtr rear;  // 队尾指针
}LinkQueue

// ----- 基本操作函数原型 -----
Status InitQueue(LinkQueue &Q)
Status DestroyQueue(LinkQueue &Q)
Status ClearQueue(LinkQueue &Q)
Status QueueEmpty(LinkQueue Q)
int QueueLength(LinkQueue Q)
Status GetHead(LinkQueue Q, ElemType &e) // 若队列 Q 不空，用 e 返回队头元素，并返回 OK；否则返回 ERROR
Status EnQueue(LinkQueue &Q, ElemType e) // 插入元素 e 为队列 Q 的新队尾元素
Status DeQueue(LinkQueue &Q, ElemType &e) // 若队列 Q 不空，则删除队头元素，用 e 返回队头元素，并返回 OK；否则返回 ERROR
Status QueueTraverse(LinkQueue Q, visit())

// ----- 基本操作算法描述 -----
Status InitQueue(LinkQueue &Q) {
    Q.front = Q.rear = (QueuePtr)malloc(sizeof(QNode));
    if(!Q.front) exit (OVERFLOW);
    Q.front->next = NULL;
    return OK;
}
Status DestroyQueue(LinkQueue &Q) {
    while (Q.front) {
        p = Q.front->next;
        free(Q.front);
        Q.front = p; // 和栈类似，这就代表一个空队列
    }
    return OK;
}
Status EnQueue(LinkQueue &Q, ElemType e) {
    QueuePtr p = (QueuePtr)malloc(sizeof(QNode));
    if(!p) exit (OVERFLOW);
    p->data = e; p->next = NULL;
    Q.rear -> next = p; Q.rear = p;
    return OK;
}
Status DeQueue(LinkQueue &Q, ElemType &e) {
    if (Q.front == Q.rear) return ERROR;
    QueuePtr p = Q.front->next;
    e = p->data;
    Q.front->next=p->next;
    if (Q.rear == p) Q.rear = front; // 和栈类似，这就代表一个空队列
    free (p);
    return OK;
}
```

> 为了高效利用存储空间，队列的顺序存储结构往往采用循环队列的方式实现（与栈直接动态分配内从不同），你能理解这是什么原因吗？

## 串

**串**是由0个或者多个字符（元素对象有所限定）组成的有限序列

相关概念：**子串**（连续），**子序列**（不要求连续），**主串**/**模式串**，**空串** $\neq$ **空格串**

抽象数据类型
```
ADT String {
    数据对象：D = { a_i | a_i ∈ CharacterSet, i = 1,2,...,n, n ≥ 0 }
    数据关系：R = { <a_i, a_{i+1}> | i = 1,2,...,n-1 }
    基本操作：
        StrAssign(&T, chars)
            初始条件：chars 是字符串常量
            操作结果：生成一个值等于 chars 的串 T
        StrCopy(&T, S)
            初始条件：串 S 存在
            操作结果：由 S 复制得到串 T
        StrEmpty(S)
            初始条件：串 S 存在
            操作结果：若 S 为空串，返回 TRUE，否则 FALSE
        StrLength(S)
            初始条件：串 S 存在
            操作结果：返回 S 的长度
        StrCompare(S, T)
            初始条件：串 S 和 T 存在
            操作结果：
                若 S > T，返回值 > 0
                若 S = T，返回 0
                若 S < T，返回值 < 0
        ClearString(&S)
            初始条件：串 S 存在
            操作结果：将 S 清为空串
        Concat(&T, S1, S2)
            初始条件：串 S1 和 S2 存在
            操作结果：用 T 返回由 S1 和 S2 连接而成的新串
        SubString(&Sub, S, pos, len)
            初始条件：
                串 S 存在，1 ≤ pos ≤ StrLength(S)，且 0 ≤ len ≤ StrLength(S)-pos+1
            操作结果：用 Sub 返回串 S 中第 pos 个字符起长度为 len 的子串
        Index(S, T)
            初始条件：串 S 和 T 存在
            操作结果：
                若主串 S 中存在和串 T 值相同的子串，返回其位置
                否则返回 0（或 -1）
        DestroyString(&S)
            初始条件：串 S 存在
            操作结果：销毁串 S
}
```

存储方式：*定长顺序存储*（数组或者结构体，串联接和求子串等串操作依赖于字符序列的复制），*堆分配*（动态内存分配，串操作依赖于字符序列的复制），*块链存储*（由固定长度的块链接而成）

`Index(S, T)` 的实现 - 模式匹配算法 值得一提：
1. Brute Force (BF)
    ```C
    int Index(char S[], char T[]) {
        int i = 0, j = 0;
        while (i < S.length && j < T.length) {
            if (S[i] == T[j]) {
                i++, j++;
            } else {
                i = i - j + 1;
                j = 0;
            }        
        }
        if (j == T.length) return i - T.length;
        return -1;
    }
    ```
    时间复杂度：$O(S.length \times T.length)$
2. KMP-algorithm

   原理：通过引入 `next[T.length]` 数组，记忆“已匹配信息”，避免回溯主串
    ```
    next[j:0,...,T.length-1] 表示：
    当模式串第 j 位失配时，下一步应该跳到哪里继续匹配
    next[j] 的本质是：
    从 T[0] 到 T[j] 这段子串中，真前缀=真后缀 的最长长度
    ```
    ```C
    int Index(char S[], char T[]) {
        int i = 0, j = 0; int next[T.length+1];
        getNext(T[], next[]);
        while (i < S.length && j < T.length) {
            if (j == 0 || S[i] == T[j]) {
                i++, j++;
            } else {
                j = next[j];
            }        
        }
        if (j == T.length) return i - T.length;
        return -1;
    }
    ```
    时间复杂度：$O(S.length + T.length)$

    > 上述 KMP 算法中 `next[]` 数组的求取算法尚未实现，你能想一想它是怎么实现的吗？
    
## 数组和广义表

每个元素都可以是一个数据结构

# 树形结构

## 树

```
ADT Tree {
    数据对象：
        D = { ai | ai ∈ ElemSet, i = 1, 2, ..., n, n ≥ 0 }

    数据关系：
        R = { <ai, aj> | ai 是 aj 的双亲（父节点），aj 是 ai 的孩子节点 }
        满足：
            1. 有且仅有一个结点没有双亲 → 根结点
            2. 其余结点有且仅有一个双亲
            3. 可以有 0 个或多个孩子
            4. 不存在回路（是层次结构）

    基本操作：
        InitTree(&T)
            操作结果：构造一棵空树 T

        DestroyTree(&T)
            操作结果：销毁树 T

        CreateTree(&T, definition)
            操作结果：按给定定义创建树

        ClearTree(&T)
            操作结果：清空树（保留结构）

        TreeEmpty(T)
            初始条件：树存在
            操作结果：若树为空返回 TRUE，否则 FALSE

        TreeDepth(T)
            操作结果：返回树的深度

        Root(T)
            操作结果：返回树的根结点

        Value(T, cur_e)
            操作结果：返回结点 cur_e 的值

        Assign(T, &cur_e, value)
            操作结果：给结点赋值

        Parent(T, cur_e)
            操作结果：返回 cur_e 的双亲

        LeftChild(T, cur_e)
            操作结果：返回 cur_e 的第一个孩子

        RightSibling(T, cur_e)
            操作结果：返回 cur_e 的右兄弟

        InsertChild(T, p, i, c)
            操作结果：在结点 p 的第 i 个位置插入子树 c

        DeleteChild(T, p, i)
            操作结果：删除结点 p 的第 i 个子树

        Traverse(T)
            操作结果：按某种方式遍历树（如先序、后序、层序等）
}
```

> 以下是树的一些相关概念，读者可以自查是否清晰：
> 结点的度，树的度，叶子结点（终端结点），孩子，双亲，兄弟，堂兄弟，结点的层次，树的深度，有序树/无序树，森林

## 二叉树

```
ADT BinaryTree {

    数据对象：
        D = { ai | ai ∈ ElemSet, i = 1,2,...,n, n ≥ 0 }

    数据关系：
        R = { <ai, aj> }

        满足：
            1. 有且仅有一个根结点
            2. 每个结点最多有两棵子树
            3. 子树有左右之分，次序不能颠倒
            4. 二叉树可为空
            5. 每棵子树本身又是一棵二叉树（递归结构）

    基本操作：

        InitBiTree(&T)
            操作结果：构造空二叉树 T

        DestroyBiTree(&T)
            操作结果：销毁二叉树 T

        CreateBiTree(&T)
            操作结果：创建二叉树

        ClearBiTree(&T)
            操作结果：清空二叉树

        BiTreeEmpty(T)
            操作结果：若 T 为空返回 TRUE，否则 FALSE

        BiTreeDepth(T)
            操作结果：返回二叉树深度

        Root(T)
            操作结果：返回根结点

        Value(T, e)
            操作结果：返回结点 e 的值

        Assign(T, e, value)
            操作结果：修改结点值

        Parent(T, e)
            操作结果：返回双亲结点

        LeftChild(T, e)
            操作结果：返回左孩子

        RightChild(T, e)
            操作结果：返回右孩子

        LeftSibling(T, e)
            操作结果：返回左兄弟

        RightSibling(T, e)
            操作结果：返回右兄弟

        InsertChild(T, p, LR, c)
            操作结果：
                在结点 p 插入子树 c
                LR=0 插入左子树
                LR=1 插入右子树

        DeleteChild(T, p, LR)
            操作结果：
                删除 p 的左/右子树

        PreOrderTraverse(T)
            先序遍历

        InOrderTraverse(T)
            中序遍历

        PostOrderTraverse(T)
            后序遍历

        LevelOrderTraverse(T)
            层序遍历
}
```

二叉树的分类：*满二叉树*，*完全二叉树*，*非完全二叉树*等

> 关于二叉树的性质，主要描述其结点个数与深度的关系，读者可以自推（假定二叉树的根节点的层次为1，深度为1）

存储结构：*顺序存储*和*链式存储*

> 顺序存储：对于一般的二叉树，应将其与完全二叉树对应，不存在的结点用 `0` 或其他字符表示
> 链式存储：通过链表来实现，不过链表的每一个结点至少包含3个域：数据域+左指针(lchild)+右指针(rchild)（这样的结构称为*二叉链表*），有些结点结构还会添加指向parent的指针域（这样的结构称为*三叉链表*）
> 注：在链式存储中，含有 n 个结点的二叉链表中有 n+1 个空链域，当这些空链域用来存储其他有用信息（该结点的前驱或者后继，并且往往是遍历而非实际二叉树结构意义上的），就得到了另一种链式存储结构——*线索链表*或*线索二叉树*

遍历二叉树的方法：先序(DLR)、中序(LDR)、后序(LRD)、层序(⬆⬇⬅➡)遍历

> D-表示根节点，L/R-表示左右子树，可见定义是递归的

每一棵树都与一棵二叉树一一对应（对应法则：对于树中的每个结点，兄弟结点按从左到右的次序依次映射到二叉树中的右子结点，子结点映射到二叉树中其父结点的左子结点）

Huffman树/最优二叉树：叶节点带权路径查毒最短的树(i.e. s.t. $\min{WPL} = \min{\sum_{k=1}^{n} \omega_k \cdot l_k}$, $\omega_k$为叶子结点上的权，$l_k$为对应叶子结点的层数-1，即根结点到叶子结点的路径长度)

> 以 4 个叶子结点的二叉树为例很容易发现构造 Huffman 树的方法，请读者试一试

# 图状结构/网状结构

## 图

```
ADT Graph {
  数据对象V：
    V = { v | v ∈ 顶点集合 }，顶点是具有相同特性的数据元素的集合。
  数据关系R：
        R = {VR}
        VR={<v,w> | v,w ∈ V 且 P(v,w), <v,w> 表示从 v 到 w 的弧，
                   谓词P(v,w)定义了弧<v,w>的意义或信息}
  基本操作P：
    // 结构创建与销毁
    CreateGraph(&G, V, VR)     // 创建图 G，V 是图的顶点集，VR 是图中弧的集合
    DestroyGraph(&G)           // 销毁图 G

    // 顶点操作
    LocateVertex(G, v)         // 返回顶点 v 在图 G 中的位置（索引或指针）
    GetVertex(G, v)            // 返回顶点 v 的值
    PutVertex(&G, v, value)    // 对图 G 中的顶点 v 赋值 value
    InsertVertex(&G, v)        // 在图 G 中增加顶点 v
    DeleteVertex(&G, v)        // 删除图 G 中的顶点 v 及其关联的弧

    // 边的操作
    InsertArc(&G, v, w, info)  // 插入从 v 到 w 的弧，info 为权值或其他信息
    DeleteArc(&G, v, w)        // 删除弧<v, w>

    // 查询与遍历
    FirstAdjVex(G, v)          // 返回 v 的第一个邻接顶点
    NextAdjVex(G, v, w)        // 返回 v 的相对于 w 的下一个邻接顶点
    // GetArcInfo(G, v, w)        // 返回弧 <v, w> 上的信息（如权值）
    // ArcExist(G, v, w)          // 判断弧 <v, w> 是否存在

    // 图的遍历
    DFSTraverse(G, Visit())    // 深度优先遍历图 G，对每个顶点调用 Visit()
    BFSTraverse(G, Visit())    // 广度优先遍历图 G，对每个顶点调用 Visit()

    // 其他辅助操作
    GetVertexNum(G)            // 返回顶点数
    GetArcNum(G)               // 返回弧数
    IsEmpty(G)                 // 判断图是否为空（无顶点）
    ClearGraph(&G)             // 清空所有顶点和边，变为空图
} ADT Graph
```

图的分类：*有向图*(以 `<v,w> ∈ VR` 代表有向的弧)，*无向图*(以 `{(v,w) ∈ VR} == {<v,w>, <w,v> ∈ VR}` 表示无向的边)，*完全图*($\frac{1}{2} n(n-1)$条边)/完全有向图($n(n-1)$条弧)，*连通图*和*强连通图*(有向图)

相关概念：顶点的*度*，入度和出度（有向图），*路径*，回路或环，简单路径，简单回路，*连通*，强连通分量，*生成树*（极小连通子图，含有图中的全部顶点，但只有n-1条边，再添加一条边则构成一个环）

存储结构：数组(邻接矩阵)，邻接表（链式存储，类似于树的链式存储，为每个图中的顶点创建一个单链表，以存储并指示与该顶点直接相邻的顶点或弧/边的信息），十字链表（链式存储，邻接表+逆邻接表），邻接多重表（链式存储，结构和十字链表类似）

遍历图的方法：深度优先搜索(DFS)，广度优先搜索(BFS)

```C
Boolean visited[MAX_VERTEX_NUM]; /* 访问标志数组(全局量) */
void(*VisitFunc)(char* v); /* 函数变量(全局量) */
void DFS(ALGraph G,int v)
{ /* 从第v个顶点出发递归地深度优先遍历图G。算法7.5 */
  int w;
  visited[v]=TRUE; /* 设置访问标志为TRUE(已访问) */
  VisitFunc(G.vertices[v].data); /* 访问第v个顶点 */
  for(w=FirstAdjVex(G,G.vertices[v].data); w>=0; w=NextAdjVex(G,G.vertices[v].data,G.vertices[w].data))
    if(!visited[w])
      DFS(G,w); /* 对v的尚未访问的邻接点w递归调用DFS */
}

void DFSTraverse(ALGraph G,void(*Visit)(char*))
{ /* 对图G作深度优先遍历。算法7.4 */
  int v;
  VisitFunc=Visit; /* 使用全局变量VisitFunc，使DFS不必设函数指针参数 */
  for(v=0;v<G.vexnum;v++)
    visited[v]=FALSE; /* 访问标志数组初始化 */
  for(v=0;v<G.vexnum;v++)
    if(!visited[v])
      DFS(G,v); /* 对尚未访问的顶点调用DFS */
  printf("\n");
}
```

```C
typedef int QElemType; /* 队列元素类型 */
#include"c3-2.h" /* 链队列的存储结构 */
#include"bo3-2.c" /* 链队列的基本操作 */
void BFSTraverse(ALGraph G,void(*Visit)(char*))
{/*按广度优先非递归遍历图G。使用辅助队列Q和访问标志数组visited。算法7.6 */
  int v,u,w;
  LinkQueue Q;
  for(v=0;v<G.vexnum;++v)
    visited[v]=FALSE; /* 置初值 */
  InitQueue(&Q); /* 置空的辅助队列Q */
  for(v=0;v<G.vexnum;v++) /* 如果是连通图，只v=0就遍历全图 */
    if(!visited[v]) /* v尚未访问 */
    {
      visited[v]=TRUE;
      Visit(G.vertices[v].data);
      EnQueue(&Q,v); /* v入队列 */
      while(!QueueEmpty(Q)) /* 队列不空 */
      {
        DeQueue(&Q,&u); /* 队头元素出队并置为u */
        for(w=FirstAdjVex(G,G.vertices[u].data);w>=0;w=NextAdjVex(G,G.vertices[u].data,G.vertices[w].data))
          if(!visited[w]) /* w为u的尚未访问的邻接顶点 */
          {
            visited[w]=TRUE;
            Visit(G.vertices[w].data);
            EnQueue(&Q,w); /* w入队 */
          }
      }
    }
  printf("\n");
}
```

连通性 - 求（无向）图的最小生成树：Prim 算法(加点法)，Kruskal 算法(加边法)

> 在边带权的无向图($n$ 个顶点)中寻找最小生成树的一个直观原理是：最终的最小生成树的边权的集合($n-1$ 条边)最好是原来无向图中边权集合(最多 $\frac{1}{2} n(n-1)$ 条边)的最小一部分
> 理解这个就很容易从初始状态(0个结点或0条边)的树构造一个边带权无向图的最小生成树了
> Tips: 
> Prim 算法是从初始状态开始，每次寻找和已有顶点(-> 即未来最小生成树的结点)集合相邻节点中边权最小的顶点加入树；Kruskal 算法则是从初始状态开始，每次寻找边权最小的边(连带着边的顶点)加入树；当然，2个算法过程中都应该考虑不要出现环

连通性 - 求（有向）图的最短路径问题：Dijkstra 算法

> 算法思路
> 0. 初始状态：路径中只有初始顶点(源点)，所有顶点到源点的距离为 $\infty$
> 1. 每次从未标记的顶点中选择距离出发点最近的顶点，标记，加入到最优路径集合中
> 2. 计算刚加入的顶点A的邻近节点B的距离，更新源点到顶点B的距离和路径
> 3. 当标记完终点时，算法结束