#include <iostream>
#include <string>
#include <stack_c.h>
#include <queue_c.h>

// ==================== 1. Bracket Matching ====================

/**
 * @brief Determines whether a pair of brackets match.
 *
 * @param open Opening bracket character.
 * @param close Closing bracket character.
 * @return true If the two characters form a matching pair.
 * @return false Otherwise.
 */
bool IsMatchingPair(char open, char close) {
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}');
}

/**
 * @brief Checks whether every bracket in a string is properly matched.
 *
 * Scans the expression from left to right, pushing every opening bracket onto
 * a C-style sequential stack. On encountering a closing bracket, it verifies
 * that it matches the top of the stack; otherwise the expression is invalid.
 *
 * @param expr The input string containing brackets (and other characters).
 * @return true If all brackets are correctly matched and balanced.
 * @return false If there is a mismatch or an unclosed bracket.
 */
bool BracketMatch(const std::string &expr) {
    StackCStyle::SqStack S;
    StackCStyle::InitStack(S);

    for (char ch : expr) {
        if (ch == '(' || ch == '[' || ch == '{') {
            StackCStyle::Push(S, static_cast<int>(ch)); // Push opening bracket
        } else if (ch == ')' || ch == ']' || ch == '}') {
            StackCStyle::ElemType topVal;
            if (StackCStyle::Pop(S, topVal)) {
                if (!IsMatchingPair(static_cast<char>(topVal), ch)) {
                    return false; // Brackets do not match
                }
            } else {
                return false; // Closing bracket with no matching opener
            }
        }
    }

    return StackCStyle::StackEmpty(S); // All openers must have been matched
}

// ==================== 2. Expression Evaluation ====================

/**
 * @brief Returns the precedence of an arithmetic operator.
 *
 * @param op The operator character ('+', '-', '*', '/', '(' or ')').
 * @return int Precedence value, where '*' and '/' rank higher than '+' and '-'.
 */
int GetPriority(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default:
            return 0; // Parentheses or invalid operator
    }
}

/**
 * @brief Converts an infix arithmetic expression into its postfix (RPN) form.
 *
 * Uses a C-style sequential stack to hold operators while emitting operands
 * directly to the output. Single-digit operands are assumed.
 *
 * @param infix The input infix expression, e.g. "3+(2-5)*6/3".
 * @return std::string The equivalent postfix expression.
 */
std::string InfixToPostfix(const std::string &infix) {
    std::string postfix;
    StackCStyle::SqStack S;
    StackCStyle::InitStack(S);

    for (char ch : infix) {
        if (ch >= '0' && ch <= '9') {
            postfix += ch; // Emit operand directly
        } else if (ch == '(') {
            StackCStyle::Push(S, static_cast<int>(ch));
        } else if (ch == ')') {
            StackCStyle::ElemType topVal;
            while (StackCStyle::Pop(S, topVal) && static_cast<char>(topVal) != '(') {
                postfix += static_cast<char>(topVal); // Pop operators until '('
            }
        } else { // Operator
            StackCStyle::ElemType topVal;
            while (StackCStyle::GetTop(S, topVal) &&
                   GetPriority(static_cast<char>(topVal)) >= GetPriority(ch)) {
                StackCStyle::Pop(S, topVal);
                postfix += static_cast<char>(topVal);
            }
            StackCStyle::Push(S, static_cast<int>(ch));
        }
    }

    // Pop any remaining operators
    StackCStyle::ElemType topVal;
    while (StackCStyle::Pop(S, topVal)) {
        postfix += static_cast<char>(topVal);
    }

    return postfix;
}

/**
 * @brief Evaluates a postfix (RPN) arithmetic expression.
 *
 * Pushes operands onto a C-style sequential stack and, on each operator,
 * pops the two most recent operands, applies the operator, and pushes the
 * result back. Single-digit operands are assumed.
 *
 * @param postfix The input postfix expression, e.g. "325-6*3/+".
 * @return int The evaluated integer result.
 */
int EvaluatePostfix(const std::string &postfix) {
    StackCStyle::SqStack S;
    StackCStyle::InitStack(S);

    for (char ch : postfix) {
        if (ch >= '0' && ch <= '9') {
            StackCStyle::Push(S, ch - '0'); // Push numeric operand
        } else {
            StackCStyle::ElemType b, a;
            StackCStyle::Pop(S, b);
            StackCStyle::Pop(S, a);
            int result = 0;
            switch (ch) {
                case '+': result = a + b; break;
                case '-': result = a - b; break;
                case '*': result = a * b; break;
                case '/': result = a / b; break;
                default: break;
            }
            StackCStyle::Push(S, result);
        }
    }

    StackCStyle::ElemType finalResult;
    StackCStyle::Pop(S, finalResult);
    return finalResult;
}

// ==================== 3. Stack in Recursion ====================

/**
 * @brief Computes the factorial of a non-negative integer using recursion.
 *
 * Relies on the implicit call stack maintained by the compiler.
 *
 * @param n A non-negative integer.
 * @return long long The value of n!.
 */
long long FactorialRecursive(int n) {
    if (n <= 1) {
        return 1;
    }
    return static_cast<long long>(n) * FactorialRecursive(n - 1);
}

/**
 * @brief Computes the factorial of a non-negative integer using an explicit stack.
 *
 * Simulates the call stack of the recursive version: it pushes n, n-1, ..., 1
 * onto a C-style sequential stack, then multiplies them while popping.
 *
 * @param n A non-negative integer.
 * @return long long The value of n!.
 */
long long FactorialWithStack(int n) {
    StackCStyle::SqStack S;
    StackCStyle::InitStack(S);

    // Push all multipliers to mimic the pending recursive calls
    for (int i = n; i > 1; i--) {
        StackCStyle::Push(S, i);
    }

    long long result = 1;
    StackCStyle::ElemType val;
    while (StackCStyle::Pop(S, val)) {
        result *= val; // Unwind the "call stack" by multiplying
    }

    return result;
}

// ==================== 4. Level-Order Traversal of a Binary Tree ====================

/**
 * @brief Binary tree node stored in an array, with child links expressed as
 *        indices into that array (1-based, 0 is reserved).
 */
struct BinTreeNode {
    int data;
    int left;  // Index of the left child, -1 if absent
    int right; // Index of the right child, -1 if absent

    /**
     * @brief Constructs a leaf node with the given value.
     *
     * @param val The value stored in the node.
     */
    explicit BinTreeNode(int val) : data(val), left(-1), right(-1) {}
};

/**
 * @brief Performs breadth-first (level-order) traversal of a binary tree.
 *
 * Uses a C-style circular queue to visit nodes layer by layer. Because the
 * queue stores plain integers, nodes are referenced by their index in the
 * array, and each node's data is printed separated by spaces.
 *
 * @param nodes The array holding the tree nodes (index 0 is unused).
 * @param root The index of the root node.
 */
void LevelOrderTraversal(const BinTreeNode nodes[], int root) {
    QueueCStyle::SqQueue Q;
    QueueCStyle::InitQueue(Q);
    QueueCStyle::EnQueue(Q, root);

    while (!QueueCStyle::QueueEmpty(Q)) {
        QueueCStyle::ElemType idx;
        QueueCStyle::DeQueue(Q, idx);

        std::cout << nodes[idx].data << " ";

        if (nodes[idx].left != -1) {
            QueueCStyle::EnQueue(Q, nodes[idx].left);
        }
        if (nodes[idx].right != -1) {
            QueueCStyle::EnQueue(Q, nodes[idx].right);
        }
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Applications of stack and queue!" << std::endl;
    std::cout << "--------------------------------" << std::endl;

    // --- 1. Bracket Matching ---
    std::cout << "[1] Bracket Matching" << std::endl;
    std::string expr1 = "{[()]}";
    std::string expr2 = "{[(])}";
    std::string expr3 = "[]{})(([))";
    std::string expr4 = "[]{}(){[(){()}]}";
    std::cout << "  \"" << expr1 << "\" -> " << (BracketMatch(expr1) ? "Matched" : "Not Matched") << std::endl;
    std::cout << "  \"" << expr2 << "\" -> " << (BracketMatch(expr2) ? "Matched" : "Not Matched") << std::endl;
    std::cout << "  \"" << expr3 << "\" -> " << (BracketMatch(expr3) ? "Matched" : "Not Matched") << std::endl;
    std::cout << "  \"" << expr4 << "\" -> " << (BracketMatch(expr4) ? "Matched" : "Not Matched") << std::endl;

    // --- 2. Expression Evaluation (Infix -> Postfix -> Value) ---
    std::cout << "[2] Expression Evaluation" << std::endl;
    std::string infix = "3+(2-5)*6/3";
    std::string postfix = InfixToPostfix(infix);
    int value = EvaluatePostfix(postfix);
    std::cout << "  Infix:   " << infix << std::endl;
    std::cout << "  Postfix: " << postfix << std::endl;
    std::cout << "  Value:   " << value << std::endl;

    // --- 3. Stack in Recursion ---
    std::cout << "[3] Stack in Recursion" << std::endl;
    int n = 6;
    std::cout << "  " << n << "! (recursive) = " << FactorialRecursive(n) << std::endl;
    std::cout << "  " << n << "! (stack)     = " << FactorialWithStack(n) << std::endl;

    // --- 4. Level-Order Traversal of a Binary Tree ---
    std::cout << "[4] Level-Order Traversal of a Binary Tree" << std::endl;
    /*
     * Build the sample tree (array-based, 1-based indices):
     *
     *          1
     *         / \
     *        2   3
     *       / \   \
     *      4   5   6
    */
    BinTreeNode nodes[7] = {
        BinTreeNode(0), // Unused sentinel at index 0
        BinTreeNode(1),
        BinTreeNode(2),
        BinTreeNode(3),
        BinTreeNode(4),
        BinTreeNode(5),
        BinTreeNode(6),
    };
    nodes[1].left = 2;
    nodes[1].right = 3;
    nodes[2].left = 4;
    nodes[2].right = 5;
    nodes[3].right = 6;

    std::cout << "  Level order: ";
    LevelOrderTraversal(nodes, 1);

    return 0;
}
