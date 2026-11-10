# Problem 6 — Average Berkeley CS Major

**Points:** 8 · **Rank:** 2 · **Problem ID:** `schedule` · **Solution:** [`solution.py`](solution.py)

> 📘 **Learning notes:** concepts, analogies and worked examples are in [LEARN.md](LEARN.md). See also the [Learning Guide](../LEARNING_GUIDE.md).

## Problem
There are `N` courses, `M` semesters and at most `K` courses per semester. Each course has at most one prerequisite, which must be taken in an **earlier** semester. Find the maximum number of courses you can take.

## Key ideas
The prerequisites form a forest. Give each course a depth, with root courses at depth 1.

1. **Feasibility (Hu's level bound, reversed in time).** A prerequisite-closed set `S` fits into `M` semesters of size `K` if and only if, for every `h ≥ 1`:
   ```
   #{ v ∈ S : depth(v) ≥ h }  ≤  K · (M − h + 1)
   ```
2. **Exchange argument.** Swapping a deepest chosen leaf for a shallower available course never breaks condition 1. So some optimal answer takes **every course of depth `< D` plus some courses of depth `D`**, for some `D`.

## Approach
Count the courses at each depth (`cnt[d]`). Then sweep `D = 1, 2, …` and keep a running minimum of the slack in condition 1. At each `D` you can take `x = min(cnt[D], min_slack)` courses from layer `D`. The answer is the best value of `taken + x`. Stop when a layer can't be fully taken or the slack goes negative.

## Code walkthrough
1. **Depths without recursion:** for each course whose depth isn't known yet, walk up the prerequisite chain until reaching a root or a course whose depth is already known. Then assign depths back down the chain. Each course is handled once.
2. **Layer sizes:** `cnt[d]` is the number of courses at depth `d`.
3. **Sweep over `D`:**
   ```python
   new_slack = K * (M - D + 1)                         # slack of level h = D
   min_slack = min(min_slack - cnt[D-1], new_slack)    # earlier levels lose cnt[D-1]
   x = min(cnt[D], min_slack)                          # depth-D courses that fit
   best = max(best, taken + x)
   ```
   - `min_slack` is the smallest value of `K·(M−h+1) − (courses already taken at depth ≥ h)` over all levels `h ≤ D`. It is updated in `O(1)` per step instead of being recomputed.
   - The loop stops when `min_slack < 0` (taking every course above `D` is already infeasible) or when layer `D` can't be taken completely (so deeper layers are impossible). It also stops at `D = M`, because a depth-`D` course needs at least `D` semesters.

## Complexity
`O(N)` per test case.

## Run
```bash
python solution.py < data/sample/00_main.in
```
