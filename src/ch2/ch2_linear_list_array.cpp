#include <iostream>
#include <linear_list_array.h>
#include <cstdlib>
#include <ostream>
#include <utility>

/**
 * @brief Application Q1: Delete the element with the minimum value and fill its gap with the last element.
 * @param L Reference to the sequential list
 * @param value Reference to a variable to store the deleted minimum value
 * @return True if successful, false otherwise
 */
bool Del_Min(SqList &L, ElemType &value){
    // 1. Check if the list is empty
    if (L.length == 0) {
        std::cerr << "Error: The sequential list is empty!" << std::endl;
        exit(EXIT_FAILURE); // Display error and terminate execution
    }
    // 2. Assume the first element is the minimum initially
    int minIndex = 0;
    value = L.data[0];
    // 3. Loop through the list to find the actual minimum element
    for (int i = 1; i < L.length; i++) {
        if (L.data[i] < value) {
            value = L.data[i];
            minIndex = i;
        }
    }
    // 4. Fill the gap with the last element of the list
    L.data[minIndex] = L.data[L.length - 1];
    // 5. Decrease the list length by 1
    //L.length--;

    return true;
}

/**
 * @brief Application Q2: Reverse all elements in the sequential list in-place.
 *        Time Complexity: O(n), Space Complexity: O(1)
 * @param L Reference to the sequential list
 */
void Reverse(SqList &L) {
    if (L.length <= 1) return; // 0 or 1 element doesn't need reversal

    int left{0};
    int right{L.length - 1};
    
    while (left < right) {
        // Swap the elements in the postion left and right
        L.data[left]  = L.data[left] + L.data[right];
        L.data[right] = L.data[left] - L.data[right];
        L.data[left]  = L.data[left] - L.data[right];
        // Move pointers to the center
        left++;
        right--;
    }

}

/**
 * @brief Application Q3: Delete all elements with value x from the sequential list.
 *        Time Complexity: O(n), Space Complexity: O(1)
 * @param L Reference to the sequential list
 * @param x The target value to be deleted
 */
void DeleteAllX(SqList &L, ElemType x) {
    int k = 0; // Index for elements not equal to x
    // Traverse the list
    for (int i = 0; i < L.length; i++) {
        if (L.data[i] != x) {
            L.data[k] = L.data[i];
            k++; // Move the slow pointer forward
        }
    }
    // Update the final length of the list
    L.length = k;
}

/**
 * @brief Application Q4: Delete elements in range [s, t].
 *        Time Complexity: O(n), Space Complexity: O(1)
 * @param L Reference to the sequential list
 * @param s Lower bound (inclusive)
 * @param t Upper bound (inclusive)
 */
void Del_Range(SqList &L, ElemType s, ElemType t) {
    // 1. Error handling: empty list or invalid range
    if (L.length == 0 || s >= t) {
        std::cerr << "Error: Invalid range (s >= t) or empty list!" << std::endl;
        exit(EXIT_FAILURE); // Display error and terminate execution
    }
    int k = 0; // Index for valid elements
    // 2. Iterate and keep only elements outside [s, t]
    for (int i = 0; i < L.length; i++) {
        if (L.data[i] < s || L.data[i] > t) {
            L.data[k] = L.data[i];
            k++;
        }
    }
    // Update the final length of the list
    L.length = k;
}

/**
 * @brief Application Q5: Delete duplicate elements from a SORTED sequential list.
 *        Time Complexity: O(n), Space Complexity: O(1)
 * @param L Reference to the sorted sequential list
 * @return True if successful, false otherwise
 */
bool Del_Same(SqList &L) {
    if (L.length == 0) {
        return false;
    }
    int k = 0;// Index for the last unique element found
    // Iterate through the list starting from the second element
    for (int i = 1; i < L.length; i++) {
        if (L.data[i] != L.data[k]) {
            k++;
            L.data[k] = L.data[i];
        }
    }
    // Update the final length of the list
    L.length = k+1;
    return true;
}

/**
 * @brief Application Q6: Merge two sorted sequential lists into a new sorted list.
 *        Time Complexity: O(L1.length + L2.length), Space Complexity: O(L1.length + L2.length)
 * @param L1 The first sorted sequential list
 * @param L2 The second sorted sequential list
 * @param result Reference to the sequential list that stores the merged result
 * @return True if merge is successful, false if the combined size exceeds MaxSize
 */
bool Merge(SqList &L1, SqList &L2, SqList &result) {
    // Check if the total combined size exceeds the maximum capacity of SqList
    if (L1.length + L2.length > MaxSize) return false;
    int i = 0; // Pointer for L1
    int j = 0; // Pointer for L2
    int k = 0; // Pointer for result
    // 1. Compare and merge elements from both lists until one is exhausted
    while (i < L1.length && j < L2.length) {
        if (L1.data[i] < L2.data[j]) {
            result.data[k++] = L1.data[i++];
        } else {
            result.data[k++] = L2.data[j++];
        }
    }
    // 2. Copy the remaining elements of L1, if any
    while (i < L1.length) result.data[k++] = L1.data[i++];
    // 2. Copy the remaining elements of L2, if any
    while (j < L2.length) result.data[k++] = L2.data[j++];
    // 4. Update the final length of the merged list
    result.length = k;
    return true;
}

/**
 * @brief Helper function to reverse a specific range [left, right] in the array.
 * @param A The array reference
 * @param left Start index of the range (0-based)
 * @param right End index of the range (0-based)
 */
void ReverseRange(SqList &A, int left, int right) {
    while (left < right) {
        std::swap(A.data[left], A.data[right]);
        left++;
        right--;
    }
}

/**
 * @brief Application Q7: Swap the positions of two sub-lists in array A.
 *        First sub-list has length m, second has length n.
 *        Time Complexity: O(m+n), Space Complexity: O(1)
 * @param A Reference to the sequential list
 * @param m Length of the first sub-list
 * @param n Length of the second sub-list
 */
void Exchange(SqList &A, int m, int n) {
    // 1. Reverse the first sub-list: from index 0 to m-1
    ReverseRange(A, 0, m - 1);
    // 2. Reverse the second sub-list: from index m to m+n-1
    ReverseRange(A, m, m + n - 1);
    // 3. Reverse the entire list: from index 0 to m+n-1
    ReverseRange(A, 0, m + n - 1);
}

/**
 * @brief Application Q8: Search for x in a sorted list using binary search.
 *        If found, swap with its successor. If not found, insert x to maintain order.
 *        Time Complexity: O(log n) for search, O(n) for insertion in worst case.
 *        Space Complexity: O(1)
 * @param L Reference to the sequential list
 * @param x The target value to search or insert
 */
void SearchExchangeInsert(SqList &L, ElemType x) {
    int low = 0, high = L.length - 1;
    int mid = 0;
    // 1. Binary Search
    while (low <= high) {
        mid = (high + low) / 2;
        if (L.data[mid] == x) {
            break;
        } 
        else if (L.data[mid] < x) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }
    // 2. If found x (L.data[mid] == x), swap with its successor
    if (low <= high && mid < L.length - 1 && mid >= 0) {
        std::swap(L.data[mid], L.data[mid + 1]);
    }
    // 3. If NOT found, insert x at index 'low' (or 'high + 1') to keep it sorted
    else if (low > high) {
        // Check if there's enough space in static array
        if (L.length >= MaxSize) {
            std::cerr << "Error: List is full, cannot insert." << std::endl;
            return;
        }
        // Shift elements from index 'low' to the right
        for (int i = L.length; i > low; i--) {
            L.data[i] = L.data[i-1];
        }
        // Insert x and update length
        L.data[low] = x;
        L.length++;
    }
}

/**
 * @brief Application Q9: Output the intersection of three sorted lists in reverse order.
 *        Time Complexity: O(n), Space Complexity: O(1)
 * @param A The first sorted sequential list
 * @param B The second sorted sequential list
 * @param C The third sorted sequential list
 */
void PrintIntersectionReverse(const SqList &A, const SqList &B, const SqList &C) {
    if (A.length == 0 || B.length == 0 || C.length == 0) {
        std::cerr << "Error: There exits empty list." << std::endl;
        return;
    }
    // 1. Initialize three pointers to the end of each list
    int i = A.length - 1;
    int j = B.length - 1;
    int k = C.length - 1;

    std::cout << "Reverse Intersection: [ ";

    while (i >= 0 && j >= 0 && k >= 0) {
        // Condition A: All three elements are equal
        if ((A.data[i] == B.data[j]) && (B.data[j] == C.data[k])) {
            std::cout << A.data[i] << " "; // Print directly (naturally in reverse order)
            i--, j--, k--;
        }
        // Condition B: Elements are not equal. Move the pointer pointing to the maximum value forward.
        else {
            int maxVal = std::max(A.data[i], std::max(B.data[j], C.data[k]));
            if (A.data[i] == maxVal) i--;
            else if (B.data[j] == maxVal) j--;
            else k--;
        }
    }
    
    std::cout << "]" << std::endl;
}

/**
 * @brief Application Q10 (2010 Unified Exam Real Question): 
 *        Cyclically left shift the sequential list R by p positions.
 *        Time Complexity: O(n), Space Complexity: O(1)
 * @param R Reference to the sequential list
 * @param p Number of positions to shift left (0 < p < n)
 */
void LeftShift(SqList &R, int p) {
    // Standard safety check for cyclic shifts
    if (p <= 0 || p >= R.length) {
        std::cerr << "Error: Invalid p." << std::endl;
        return;
    }
    // 1. Reverse the first sub-list: from index 0 to p-1
    ReverseRange(R, 0, p - 1);
    // 2. Reverse the second sub-list: from index p to R.length-1
    ReverseRange(R, p, R.length - 1);
    // 3. Reverse the entire list: from index 0 to R.length-1
    ReverseRange(R, 0, R.length - 1);
}

int main() {
    std::cout << "Hello, linear list!" << "\n";

    SqList L;
    InitList(L); // Initialization

    // Insertion tests
    ListInsert(L, 1, 10);
    ListInsert(L, 2, 20);
    ListInsert(L, 1, 5); // Insert 5 at the very front
    ListInsert(L, 1, 1);
    ListInsert(L, 5, 25);
    PrintList(L);        // Expected output: [ 1 5 10 20 25 ]
    // Deletion test
    int deletedVal;
    if (ListDelete(L, 2, deletedVal)) {
        std::cout << "Successfully deleted: " << deletedVal << std::endl;
    }
    // Locate test
    int locateVal{10};
    std::cout << "Element " << locateVal << " in position: " << LocateElem(L, locateVal) << std::endl;
    PrintList(L);        // Expected output: [ 1 10 20 25 ]
    
    // Application Q1
    std::cout << "===== Application Q1 =====" << std::endl;
    int del_minValue;
    Del_Min(L, del_minValue);
    std::cout << "Deleted minimal value: " << del_minValue << std::endl;
    PrintList(L);        // Expected output: [ 25 10 20 25 ]

    // Application Q2
    std::cout << "===== Application Q2 =====" << std::endl;
    Reverse(L);
    std::cout << "Reversed list: ";
    PrintList(L);        // Expected output: [ 25 20 10 25 ]
    Reverse(L);
    std::cout << "Reversed list: ";
    PrintList(L);        // Expected output: [ 25 10 20 25 ]
    
    // Application Q3
    std::cout << "===== Application Q3 =====" << std::endl;
    int x{10};
    DeleteAllX(L, x);
    std::cout << "Delete All x = " << x << ", then list is: ";
    PrintList(L);        // Expected output: [ 25 20 25 ]
    ListInsert(L, 2, x);
    x = 25;
    DeleteAllX(L, x); // Restore L: [25 10 20 25]
    std::cout << "Delete All x = " << x << ", then list is: ";
    PrintList(L);        // Expected output: [ 10 20 ]
    ListInsert(L, 1, x);
    ListInsert(L, L.length + 1, x); // Restore L: [25 10 20 25]
    
    // Application Q4
    std::cout << "===== Application Q4 =====" << std::endl;
    std::cout << "Now list is: ";
    PrintList(L);
    Del_Range(L, 10, 20);
    std::cout << "Delete All x in [10, 20], then list is: ";
    PrintList(L);
    
    // Application Q5
    std::cout << "===== Application Q5 =====" << std::endl;
    // Re-initialize the SqList L
    for (int i = 0; i < 10; i++) {
        L.data[i] = (i/2 + 1) * 10;
        L.length = 10;
    }
    std::cout << "Now list is: ";
    PrintList(L); // Excepted output: [ 10 10 20 20 30 30 40 40 50 50]
    Del_Same(L);
    std::cout << "Delete All same x in L, then list is: ";
    PrintList(L);
    
    // Application Q6
    std::cout << "===== Application Q6 =====" << std::endl;
    SqList L1, L2, result;
    InitList(L1);
    InitList(L2);
    InitList(result);

    // Prepare L1: [ 1, 3, 5, 7 ]
    for (int i = 0; i < 4; i++) {
        ListInsert(L1, i + 1, 2 * i + 1);
    }

    // Prepare L2: [ 2, 4, 6, 8, 10 ]
    for (int j = 0; j < 5; j++) {
        ListInsert(L2, j + 1, 2 * (j + 1));
    }

    std::cout << "List 1: ";
    PrintList(L1); // Expected: [ 1 3 5 7 ]
    std::cout << "List 2: ";
    PrintList(L2); // Expected: [ 2 4 6 8 10 ]
    // Merge L1 and L2
    if (Merge(L1, L2, result)) {
        std::cout << "Merged List: ";
        PrintList(result); // Expected: [ 1 2 3 4 5 6 7 8 10 ]
    } else {
        std::cerr << "Error: Merged size exceeds max capacity." << std::endl;
    }
    
    // Application Q7
    std::cout << "===== Application Q7 =====" << std::endl;
    SqList A;
    InitList(A);

    int m = 3; // Length of the first sub-list:  [ 1, 2, 3 ]
    int n = 4; // Length of the second sub-list: [ 10, 20, 30, 40 ]

    // Initialize list A with m + n elements: [ 1, 2, 3, 10, 20, 30, 40 ]
    for (int i = 0; i < 3; i++) ListInsert(A, i + 1, i + 1);
    for (int j = 0; j < 4; j++) ListInsert(A, A.length + 1, (j + 1) * 10);

    std::cout << "Before Exchanging: ";
    PrintList(A); // Expected: [ 1 2 3 10 20 30 40 ]

    // Perform the exchange
    Exchange(A, m, n);

    std::cout << "After Exchanging: ";
    PrintList(A); // Expected: [ 10 20 30 40 1 2 3 ]
    
    // Application Q8
    std::cout << "===== Application Q8 =====" << std::endl;
    SqList B;
    InitList(B);

    // Initial sorted data: [ 10, 20, 30, 40, 50 ]
    for (int i = 0; i < 5; i++) {
        ListInsert(B, i + 1, 10 * (i + 1));
    }

    std::cout << "Initial List: ";
    PrintList(B); // Expected: [ 10 20 30 40 50 ]

    // Test 1: Found case (Search for 30, should swap with 40)
    std::cout << "--- Test 1: Search for 30 (Found) ---" << std::endl;
    SearchExchangeInsert(L, 30);
    PrintList(L); // Expected: [ 10 20 40 30 50 ]
    // Reset list back to sorted for the next test
    std::swap(L.data[2], L.data[3]); 
    // Test 2: Not found case (Search for 35, should insert between 30 and 40)
    std::cout << "--- Test 2: Search for 35 (Not Found -> Insert) ---" << std::endl;
    SearchExchangeInsert(L, 35);
    PrintList(L); // Expected: [ 10 20 30 35 40 50 ]

    // Application Q9
    SqList A1, B1, C1;
    InitList(A1); InitList(B1); InitList(C1);

    // Test data from the example:
    // A1 = {1, 2, 3}
    ListInsert(A1, 1, 1); ListInsert(A1, 2, 2); ListInsert(A1, 3, 3);
    // B1 = {2, 3, 4}
    ListInsert(B1, 1, 2); ListInsert(B1, 2, 3); ListInsert(B1, 3, 4);
    // C1 = {-1, 0, 2}
    ListInsert(C1, 1, -1); ListInsert(C1, 2, 0); ListInsert(C1, 3, 2);

    std::cout << "List A: "; PrintList(A1);
    std::cout << "List B: "; PrintList(B1);
    std::cout << "List C: "; PrintList(C1);

    std::cout << "--- Executing Q9 Algorithm ---" << std::endl;
    // Expected output: 2 (since 2 is the only common element)
    PrintIntersectionReverse(A1, B1, C1);
    
    // Application Q10
    SqList R;
    InitList(R);

    // Prepare test data R = (1, 2, 3, 4, 5, 6, 7), n = 7
    for (int i = 1; i <= 7; i++) {
        ListInsert(R, i, i);
    }
    int p = 3; // Shift left by 3 positions

    std::cout << "Before Left Shift: ";
    PrintList(R); // Expected output: [ 1 2 3 4 5 6 7 ]

    // Execute the optimal algorithm
    LeftShift(R, p);

    std::cout << "After Left Shift by " << p << " positions: ";
    PrintList(R); // Expected output: [ 4 5 6 7 1 2 3 ]
                  
    LeftShift(R, p);

    std::cout << "After Left Shift by " << p << " positions: ";
    PrintList(R); // Expected output: [ 7 1 2 3 4 5 6 ]

    return 0;
}