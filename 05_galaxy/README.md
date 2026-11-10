# Problem 5 — Super Ben Galaxy

**Points:** 7 + 2 (main + bonus) · **Rank:** 2 + 3 · **Problem ID:** `galaxy` · **Solution:** [`solution.py`](solution.py)

> 📘 **Learning notes:** concepts, analogies and worked examples are in [LEARN.md](LEARN.md). See also the [Learning Guide](../LEARNING_GUIDE.md).

## Problem
An `N × M` grid has red (`R`), blue (`B`) and normal (`.`) tiles. Normal tiles are always walkable. Red tiles are walkable only in red mode and blue tiles only in blue mode. On a normal tile you can *spin*, which toggles the mode. Each query asks whether Ben can walk from a start tile (normal, starting in blue mode) to a destination tile.

## Approach
Model the state as `(cell, mode)`, which gives `2·N·M` states. Every move and every spin can be undone, so reachability is just connectivity, and **union-find** handles it:

- Union `(cell, m)` with its neighbour `(cell', m)` when both tiles are active in mode `m`.
- On a normal tile, union `(cell, blue)` with `(cell, red)` (the spin).

A query is `YES` if the start state `(start, blue)` is in the same component as some active state of the destination tile.

## Code walkthrough
| Part | What it does |
|---|---|
| State id | `(r * M + c) * 2 + mode`, with blue = 0 and red = 1 |
| `find(x)` | Iterative DSU find with path compression, so deep chains can't hit Python's recursion limit |
| `union(a, b)` | Joins two components |
| `active(ch, mode)` | `.` is always walkable. `B` only in blue mode, `R` only in red mode. |
| Build loop | For every cell: if it is `.`, union its two modes (spin). For each mode where the cell is active, union with the **right** and **down** neighbours that are active in the same mode. Right and down is enough to cover every edge once. |
| Query loop | `s = find(start, blue)`. The answer is `YES` if the destination's blue state (when blue is active there) or red state (when red is active there) has the same root. |

## Complexity
About `O((N·M + Q) · α)`. The same code solves both the main and the bonus test sets.

## Run
```bash
python solution.py < data/sample/00_main.in
python solution.py < data/sample/01_bonus.in
```
