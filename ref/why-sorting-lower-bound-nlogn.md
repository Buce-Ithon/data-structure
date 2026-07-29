# Why Is O(n log n) the Optimal Time Complexity for Comparison-Based Sorting?

## Core Conclusion

> For **comparison-based** sorting algorithms, the best possible worst-case time complexity is **O(n log₂ n)** — a theoretically insurmountable lower bound.

---

## 1. The Decision Tree Model

Every comparison-based sorting process can be abstracted as a **binary decision tree**:

- Each comparison between two elements (a and b) yields one of two outcomes: `a < b` or `a > b`
- Based on the result, the algorithm proceeds down a different branch
- Eventually, it reaches a **leaf node**, which corresponds to a fully sorted permutation

```
          Compare a vs b
          /            \
       a < b          a > b
       /                  \
   Compare b vs c      Compare a vs c
    /        \           /        \
  ...       ...        ...       ...
```

---

## 2. How Many Cases Must Be Distinguished?

For n distinct elements, the number of possible permutations is **n!**.

Each leaf node corresponds to one final permutation, so the decision tree must have **at least n! leaves**.

A binary tree of height h can have at most **2^h** leaves. Therefore:

$$2^h \ge n!$$

Taking the logarithm:

$$h \ge \log_2(n!)$$

---

## 3. What Stirling's Approximation Tells Us

Stirling's approximation:

$$n! \approx \sqrt{2\pi n} \cdot \left(\frac{n}{e}\right)^n$$

Taking the logarithm of both sides:

$$\log_2(n!) = n\log_2 n - n\log_2 e + \frac{1}{2}\log_2(2\pi n)$$

Dropping lower-order terms and constants:

$$\log_2(n!) = \Theta(n\log n)$$

---

## 4. Conclusion

The height of the decision tree h represents the **minimum number of comparisons required in the worst case**, and h ∈ Ω(n log n).

This means: **any comparison-based sorting algorithm requires at least Ω(n log n) comparisons in the worst case**. No algorithm can break through this theoretical lower bound.

Best-case and average-case scenarios do not change the fundamental bound: the best possible worst-case time complexity for comparison-based sorting remains **O(n log n)**. Algorithms like Merge Sort and Heap Sort achieve this optimal bound.

---

## 5. Important Exceptions

This lower bound applies **only to comparison-based sorting**. If additional assumptions about the data are available, the bound can be surpassed:

| Algorithm | Time Complexity | Applicable Conditions |
|-----------|----------------|-----------------------|
| Counting Sort | O(n + k) | k is the value range; suitable for integers |
| Radix Sort | O(d · n) | Fixed-width integers or strings (d digits) |
| Bucket Sort | O(n) average | Data is uniformly distributed |

These algorithms **do not compare elements directly**. Instead, they exploit structural properties of the data (bits, digits, distribution), thereby circumventing the information-theoretic Ω(n log n) lower bound of the comparison model.

---

## Further Reading

- Knuth, D. E. — *The Art of Computer Programming, Volume 3: Sorting and Searching*
- Cormen, T. H. et al. — *Introduction to Algorithms*, Chapter 8: Sorting in Linear Time
