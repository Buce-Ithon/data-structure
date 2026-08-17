#include <iostream>
#include "stack_c.h"
#include "queue_c.h"

// ============================================================================
// Application 1: Parenthesis Matching using Stack
// ============================================================================

/**
 * @brief Checks if parentheses in an expression are balanced using a sequential stack.
 * @param expr C-style string containing the expression.
 * @return bool Returns true if balanced; returns false otherwise.
 */
bool IsParenthesesBalanced(const char* expr) {
    StackCStyle::SqStack S;
    StackCStyle::InitStack(S);

    for (int i = 0; expr[i] != '\0'; ++i) {
        char ch = expr[i];
        if (ch == '(' || ch == '[' || ch == '{') {
            StackCStyle::Push(S, ch);
        } else if (ch == ')' || ch == ']' || ch == '}') {
            if (StackCStyle::StackEmpty(S)) {
                return false;
            }
            StackCStyle::ElemType topVal;
            StackCStyle::Pop(S, topVal);
            char topChar = static_cast<char>(topVal);

            if ((ch == ')' && topChar != '(') ||
                (ch == ']' && topChar != '[') ||
                (ch == '}' && topChar != '{')) {
                return false;
            }
        }
    }
    return StackCStyle::StackEmpty(S);
}

// ============================================================================
// Application 2: Expression Evaluation (Infix -> Postfix -> Evaluate)
// ============================================================================

/**
 * @brief Helper function to get the operator precedence.
 * @param op Operator character.
 * @return int Integer priority (higher value indicates higher precedence).
 */
int GetPrecedence(char op) {
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

/**
 * @brief Converts an infix arithmetic expression into a postfix expression.
 * @param infix Source infix expression (supports single-digit operands and + - * / ( )).
 * @param postfix Output buffer to store the converted postfix expression.
 */
void InfixToPostfix(const char* infix, char* postfix) {
    StackCStyle::SqStack S;
    StackCStyle::InitStack(S);
    int j = 0;

    for (int i = 0; infix[i] != '\0'; ++i) {
        char ch = infix[i];
        if (ch >= '0' && ch <= '9') {
            postfix[j++] = ch;
        } else if (ch == '(') {
            StackCStyle::Push(S, ch);
        } else if (ch == ')') {
            StackCStyle::ElemType op;
            while (!StackCStyle::StackEmpty(S)) {
                StackCStyle::GetTop(S, op);
                if (static_cast<char>(op) == '(') {
                    StackCStyle::Pop(S, op); // Remove '('
                    break;
                }
                StackCStyle::Pop(S, op);
                postfix[j++] = static_cast<char>(op);
            }
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            StackCStyle::ElemType topOp;
            while (!StackCStyle::StackEmpty(S)) {
                StackCStyle::GetTop(S, topOp);
                char topChar = static_cast<char>(topOp);
                if (topChar == '(' || GetPrecedence(topChar) < GetPrecedence(ch)) {
                    break;
                }
                StackCStyle::Pop(S, topOp);
                postfix[j++] = topChar;
            }
            StackCStyle::Push(S, ch);
        }
    }
    while (!StackCStyle::StackEmpty(S)) {
        StackCStyle::ElemType op;
        StackCStyle::Pop(S, op);
        postfix[j++] = static_cast<char>(op);
    }
    postfix[j] = '\0';
}

/**
 * @brief Evaluates a postfix arithmetic expression using a stack.
 * @param postfix Postfix expression string.
 * @return int Calculated evaluation result.
 */
int EvaluatePostfix(const char* postfix) {
    StackCStyle::SqStack S;
    StackCStyle::InitStack(S);

    for (int i = 0; postfix[i] != '\0'; ++i) {
        char ch = postfix[i];
        if (ch >= '0' && ch <= '9') {
            StackCStyle::Push(S, ch - '0');
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            StackCStyle::ElemType operand2, operand1;
            StackCStyle::Pop(S, operand2);
            StackCStyle::Pop(S, operand1);
            switch (ch) {
                case '+': StackCStyle::Push(S, operand1 + operand2); break;
                case '-': StackCStyle::Push(S, operand1 - operand2); break;
                case '*': StackCStyle::Push(S, operand1 * operand2); break;
                case '/': StackCStyle::Push(S, operand1 / operand2); break;
            }
        }
    }
    StackCStyle::ElemType result;
    StackCStyle::Pop(S, result);
    return result;
}

// ============================================================================
// Application 3: Recursion Simulation using Stack (Factorial Example)
// ============================================================================

/**
 * @brief Calculates factorial of n by simulating recursion stack frames iteratively.
 * @param n Non-negative integer.
 * @return long long Factorial of n (n!).
 */
long long FactorialWithStack(int n) {
    if (n <= 1) return 1;

    StackCStyle::SqStack S;
    StackCStyle::InitStack(S);

    // Push call parameters onto the explicit stack
    for (int i = n; i > 1; --i) {
        StackCStyle::Push(S, i);
    }

    long long result = 1;
    // Pop parameters to compute accumulative product (simulating returning stack frames)
    while (!StackCStyle::StackEmpty(S)) {
        StackCStyle::ElemType val;
        StackCStyle::Pop(S, val);
        result *= val;
    }
    return result;
}

// ============================================================================
// Application 4: Binary Tree Level-Order Traversal using Queue
// ============================================================================

/**
 * @struct TreeNode
 * @brief Binary Tree Node structure.
 */
struct TreeNode {
    int val;         /**< Node value */
    TreeNode *left;  /**< Pointer to left child */
    TreeNode *right; /**< Pointer to right child */
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

/**
 * @struct TreeQueue
 * @brief Queue structure dedicated to storing TreeNode pointers for level-order traversal.
 */
typedef struct {
    TreeNode *data[QueueCStyle::MaxSize]; /**< Array storing TreeNode pointers */
    int front;                            /**< Head pointer */
    int rear;                             /**< Tail pointer */
} TreeQueue;

/**
 * @brief Initializes the tree node queue.
 * @param Q Reference to the tree node queue.
 */
void InitTreeQueue(TreeQueue &Q) {
    Q.front = 0;
    Q.rear = 0;
}

/**
 * @brief Checks if the tree node queue is empty.
 * @param Q Reference to the tree node queue.
 * @return bool Returns true if empty; returns false otherwise.
 */
bool TreeQueueEmpty(const TreeQueue &Q) {
    return Q.front == Q.rear;
}

/**
 * @brief Enqueues a TreeNode pointer.
 * @param Q Reference to the tree node queue.
 * @param node Pointer to the tree node.
 * @return bool Returns true if enqueued successfully; returns false if full.
 */
bool EnTreeQueue(TreeQueue &Q, TreeNode *node) {
    if ((Q.rear + 1) % QueueCStyle::MaxSize == Q.front) return false;
    Q.data[Q.rear] = node;
    Q.rear = (Q.rear + 1) % QueueCStyle::MaxSize;
    return true;
}

/**
 * @brief Dequeues a TreeNode pointer.
 * @param Q Reference to the tree node queue.
 * @param node Reference pointer variable to store dequeued tree node.
 * @return bool Returns true if dequeued successfully; returns false if empty.
 */
bool DeTreeQueue(TreeQueue &Q, TreeNode *&node) {
    if (Q.front == Q.rear) return false;
    node = Q.data[Q.front];
    Q.front = (Q.front + 1) % QueueCStyle::MaxSize;
    return true;
}

/**
 * @brief Performs level-order traversal on a binary tree using a queue.
 * @param root Pointer to the root node of the binary tree.
 */
void LevelOrderTraversal(TreeNode *root) {
    if (!root) return;

    TreeQueue Q;
    InitTreeQueue(Q);
    EnTreeQueue(Q, root);

    while (!TreeQueueEmpty(Q)) {
        TreeNode *curr = nullptr;
        DeTreeQueue(Q, curr);
        std::cout << curr->val << " ";

        if (curr->left) EnTreeQueue(Q, curr->left);
        if (curr->right) EnTreeQueue(Q, curr->right);
    }
    std::cout << std::endl;
}

// ============================================================================
// Main Function & Application Tests
// ============================================================================

int main() {
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Applications of stack and queue!" << std::endl;
    std::cout << "--------------------------------" << std::endl;

    // Application 1: Parenthesis Matching
    std::cout << "\n===== Application 1: Parenthesis Matching =====" << std::endl;
    const char* expr1 = "{[()()]}";
    const char* expr2 = "{[(])}";
    std::cout << "Expression: " << expr1 << " -> " 
              << (IsParenthesesBalanced(expr1) ? "Balanced" : "Unbalanced") << std::endl;
    std::cout << "Expression: " << expr2 << " -> " 
              << (IsParenthesesBalanced(expr2) ? "Balanced" : "Unbalanced") << std::endl;

    // Application 2: Expression Evaluation
    std::cout << "\n===== Application 2: Expression Evaluation =====" << std::endl;
    const char* infix = "(1+2)*3-8/4";
    char postfix[128];
    InfixToPostfix(infix, postfix);
    int evalResult = EvaluatePostfix(postfix);
    std::cout << "Infix Expression:   " << infix << std::endl;
    std::cout << "Postfix Expression: " << postfix << std::endl;
    std::cout << "Evaluation Result:  " << evalResult << std::endl;

    // Application 3: Recursion Simulation using Stack
    std::cout << "\n===== Application 3: Recursion Simulation =====" << std::endl;
    int n = 5;
    std::cout << "Factorial of " << n << " (with stack simulation): " << FactorialWithStack(n) << std::endl;

    // Application 4: Binary Tree Level-Order Traversal
    std::cout << "\n===== Application 4: Level-Order Traversal =====" << std::endl;
    /*
            1
           / \
          2   3
         / \
        4   5
    */
    TreeNode *root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);

    std::cout << "Level-order traversal output: ";
    LevelOrderTraversal(root);

    // Free tree node memory
    delete root->left->left;
    delete root->left->right;
    delete root->left;
    delete root->right;
    delete root;

    return 0;
}