# Learn: Problem 6 — schedule (Average Berkeley CS Major)

**Concepts:** trees and forests, depth, scheduling with precedence, Hu's level bound, greedy proofs by exchange argument · **Difficulty:** ⭐⭐⭐

---

## 1. The structure: a forest

Each course has **at most one** prerequisite. If you draw an arrow `prereq → course`, every node has at most one incoming arrow, so the picture is a **forest** (a set of rooted trees).

```
   1        2         depth 1 (no prerequisite)
  / \
 3   4                depth 2
 |
 5                    depth 3
```

**Depth** = the length of the prerequisite chain ending at that course, so roots have depth 1. A depth-`d` course can be taken in semester `d` at the earliest, since its whole chain must come first.

> **Analogy:** a recipe. You can't frost the cake (depth 3) before baking it (depth 2), and you can't bake it before mixing the batter (depth 1). Some recipes are long chains, others are many short independent steps.

## 2. Why "take whatever is available" fails (greedy pitfall)

M = 3 semesters, K = 2 per semester. Chain `1 → 3 → 4`, plus independent courses `2, 5, 6`.

| Strategy | Sem 1 | Sem 2 | Sem 3 | Total |
|---|---|---|---|---|
| Naive: pick any available | 2, 5 | 1, 6 | 3 | **5** |
| Deep chains first (Hu) | 1, 2 | 3, 5 | 4, 6 | **6** ✅ |

The naive plan delays the **long chain**, and runs out of semesters before reaching course 4.

**Hu's rule (1961):** when you can't take everything, take the courses that start the **longest remaining chain** first. It is optimal for unit-time tasks with tree-shaped precedence.

## 3. When is a set of courses feasible?

Take a set `S` that is closed under prerequisites (if a course is in `S`, so is its prerequisite).

A course of depth ≥ h cannot be taken in the first `h − 1` semesters. So all such courses must fit into the **last `M − h + 1` semesters**:

```
#{ v ∈ S : depth(v) ≥ h }  ≤  K · (M − h + 1)     for every h ≥ 1      (★)
```

- (★) is clearly **necessary**.
- Hu's theorem says it's also **sufficient** for trees (apply Hu's algorithm in reversed time, where the forest becomes an "in-tree").

> **Analogy:** a bus that leaves at fixed stops. Passengers who can only board at stop h or later must all fit in the seats offered from stop h onwards.

## 4. Which set S is best? Exchange argument

**Claim:** some optimal `S` takes **all** courses of depth `< D`, plus **some** courses of depth exactly `D`.

**Proof idea (exchange argument):** suppose an optimal `S` skips a shallow course `u` (depth `d`) but contains a deeper leaf `v` (depth `> d`) whose removal keeps `S` closed. Swap them: drop `v`, add `u`.
- The size stays the same.
- For every `h`, the left side of (★) stays the same or goes **down**, because we replaced a deep course with a shallow one. So (★) still holds.

Repeat until no such swap exists, and you get the "layers" shape. ∎

> **Exchange argument recipe:** (1) take any optimal solution, (2) show you can change it step by step toward the greedy solution, (3) without ever making it worse. Then the greedy is optimal.

## 5. Turning it into an O(N) algorithm

`cnt[d]` = number of courses at depth `d`. For a fixed `D`, we take `taken = cnt[1] + … + cnt[D−1]` plus `x` courses of depth `D`. Then (★) says, for each `h ≤ D`:

```
x ≤ K·(M − h + 1) − (cnt[h] + … + cnt[D−1])   =: slack_h
```

So `x = min(cnt[D], min_h slack_h)`. When `D` grows by 1, every old slack drops by `cnt[D−1]` and one new slack appears. That gives an **O(1) update**:

```python
min_slack = min(min_slack - cnt[D-1], K * (M - D + 1))
```

## 6. Worked examples

**Easy:** chain `1 → 2 → 3`, K = 2, M = 2

| D | new slack K(M−D+1) | min_slack | x = min(cnt[D], min_slack) | best |
|---|---|---|---|---|
| 1 | 4 | 4 | min(1, 4) = 1 | 1 |
| 2 | 2 | min(4−1, 2) = 2 | min(1, 2) = 1 | **2** |

We stop at D = M = 2, because course 3 needs 3 semesters. Answer **2**.

**Complex:** the table from section 2 (M = 3, K = 2). Depth counts: cnt[1] = 4 (courses 1, 2, 5, 6), cnt[2] = 1 (course 3), cnt[3] = 1 (course 4).

| D | new slack | min_slack | x | best | taken after |
|---|---|---|---|---|---|
| 1 | 6 | 6 | min(4, 6) = 4 | 4 | 4 |
| 2 | 4 | min(6−4, 4) = 2 | min(1, 2) = 1 | 5 | 5 |
| 3 | 2 | min(2−1, 2) = 1 | min(1, 1) = 1 | **6** | 6 |

Answer **6**, matching Hu's schedule.

## 7. Computing depth without recursion
A chain can be 2·10⁵ long, which would crash a recursive function in Python. The solution walks **up** until it reaches a known depth, stores the path, and then assigns depths **down** the path. Each course is processed once, so this is O(N).

## 8. Complexity
O(N) per test case.

## 9. Bigger picture
- With a **general DAG** (several prerequisites per course), this problem becomes NP-hard. That's why LeetCode 1494 (*Parallel Courses II*) uses bitmask DP with N ≤ 15.
- The **tree structure** is what makes a greedy possible here. Always ask which special structure the constraints give you.

## 10. Practice
- LeetCode 207 / 210: *Course Schedule I / II* (topological order)
- LeetCode 1136: *Parallel Courses* (unlimited K → the answer is the longest chain)
- LeetCode 1494: *Parallel Courses II* (the hard general version)
