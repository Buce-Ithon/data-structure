#ifndef LINEAR_LIST_ARRAY_H
#define LINEAR_LIST_ARRAY_H

// Define maxsize of capacity.
#define MaxSize 128

// Reserved element type, using int as a default example. 
// Can be changed to other types in practical use.
using ElemType = int;

// Structure definition for statically allocated sequential list
typedef struct {
    ElemType data[MaxSize]; // Static array to store elements
    int length;             // Current length of the list
} SqList;

/**
 * @brief Initializes the static sequential list
 * @param L Reference to the sequential list
 */
void InitList(SqList &L);

/**
 * @brief Inserts an element at a specific position
 * @param L Reference to the sequential list
 * @param i Insertion position (Note: 1-indexed, 1 <= i <= L.length + 1)
 * @param e The element value to be inserted
 * @return True if insertion is successful, false otherwise
 */
bool ListInsert(SqList &L, int i, ElemType e);

/**
 * @brief Deletes an element at a specific position and returns the deleted value
 * @param L Reference to the sequential list
 * @param i Deletion position (1-indexed, 1 <= i <= L.length)
 * @param e Reference to a variable to save the deleted element
 * @return True if deletion is successful, false otherwise
 */
bool ListDelete(SqList &L, int i, ElemType &e);

/**
 * @brief Finds an element by its value
 * @param L The sequential list
 * @param e The target value to search for
 * @return The 1-based index (position) of the target element, or 0 if not found
 */
int LocateElem(SqList L, ElemType e);

/**
 * @brief Prints all elements of the sequential list (for debugging purposes)
 * @param L The sequential list
 */
void PrintList(SqList L);

#endif // LINEAR_LIST_ARRAY_H