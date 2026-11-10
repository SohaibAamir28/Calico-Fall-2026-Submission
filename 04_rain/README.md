# Problem 4 — Drone Rain

**Points:** 7 · **Rank:** 2 · **Problem ID:** `rain` · **Solution:** [`solution.py`](solution.py)

> 📘 **Learning notes:** concepts, analogies and worked examples are in [LEARN.md](LEARN.md). See also the [Learning Guide](../LEARNING_GUIDE.md).

## Problem
`N` drones start at distinct `(X, Y)` positions at fixed heights `Z`, and each drone moves one unit per night either East or North. Over `K` nights, a drone collects 1 unit of rain each night unless another drone is directly above it. Find the total rain collected.

## Key observations
- Two drones moving in the **same** direction keep the same offset from each other. Since no two drones start at the same point, they never stack.
- An East drone `(Xe + t, Ye)` and a North drone `(Xn, Yn + t)` are at the same point exactly when `t = Xn − Xe = Ye − Yn`. That means `Xe + Ye = Xn + Yn` and `t = Xn − Xe`.
- So each East/North pair meets **at most once**, and at most two drones ever share a point. Each such meeting on night `t ∈ [0, K−1]` costs exactly 1 unit of rain (the lower drone gets none).

```
answer = N·K − #{ (E, N) pairs : Xe+Ye = Xn+Yn  and  0 ≤ Xn − Xe ≤ K − 1 }
```

## Approach
Group drones by the sum `X + Y`. Within a group, sort the East drones' `X` values. For each North drone, count the East drones with `Xe ∈ [Xn − K + 1, Xn]` using binary search.

## Code walkthrough
1. **Grouping:** `east[X+Y]` and `north[X+Y]` store the `X` coordinates of the East and North drones on each diagonal. The height `Z` is never needed, since in a stack one drone is always lower.
2. **Counting meetings:** for each diagonal that has both kinds of drones, sort the East list. Then for each North drone at `xn`:
   ```python
   lost += bisect_right(xs_e, xn) - bisect_left(xs_e, xn - (K - 1))
   ```
   This counts the East drones with `xn − (K−1) ≤ xe ≤ xn`, which are the ones that meet this North drone on a night `t = xn − xe` within `[0, K−1]`.
3. **Answer:** `N * K - lost`.

## Complexity
`O(N log N)` per test case.

## Run
```bash
python solution.py < data/sample/00_main.in
```
