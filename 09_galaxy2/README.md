# Problem 9 — Super Ben Galaxy 2

**Points:** 11 · **Rank:** 3 · **Problem ID:** `galaxy2` · **Solution:** [`solution.cpp`](solution.cpp)

> 📘 **Learning notes:** concepts, analogies and worked examples are in [LEARN.md](LEARN.md). See also the [Learning Guide](../LEARNING_GUIDE.md).

## Problem
Same grid and rules as [Problem 5](../05_galaxy/README.md), but a step and a spin each now take 1 second. Each query asks whether the destination can be reached within `K` seconds. The grid has at most **`N ≤ 10` rows**.

## Approach: divide and conquer on columns
This is a shortest-path problem on states `(cell, mode)`. Because the grid is narrow, split it recursively by columns.

- For a node covering columns `[l, r]` with middle column `mid`, run a BFS from every state in column `mid` (at most `2N` sources). The BFS stays inside columns `[l, r]`.
- Every query with both endpoints inside `[l, r]` updates its best value with `min over sources (d(src, start) + d(src, end))`.
- Queries with both endpoints on the left of `mid` go down to the left child, and both on the right go to the right child.

**Why it is correct:** take the deepest node whose column range contains the whole optimal path. That path must cross that node's middle column, and the restricted BFS distances there are exact. The query is evaluated at every node on its way down, so it reaches that node.

## Code walkthrough
| Part | What it does |
|---|---|
| `sid(row, col, mode)` | Local state id inside the current column range: `((col − l) * N + row) * 2 + mode` |
| `sources` | Every state in column `mid` that is active in its mode |
| `distBuf` | One distance array per source, stored back to back (`S × segSize` ints), reused between calls |
| BFS | From a state `u`, spin to `u ^ 1` if the tile is `.`, or walk to any of the 4 neighbours that is active in the same mode and lies inside `[l, r]`. Every edge costs 1, so a plain queue BFS gives shortest paths. |
| Query update | Since the graph is undirected, `d(src, start) = d(start, src)`. So `best[q] = min(best[q], d[s] + min(d[e0], d[e1]))`, using only the destination modes that are active. |
| Splitting | A query with both columns `< mid` goes left, and both `> mid` goes right. Any other query crosses `mid`, so its answer is already final. |
| `rec(l, r, qs)` | Recurses on `[l, mid−1]` and `[mid+1, r]` after freeing `qs` |

At the end, a query is `YES` if `best[q] ≤ K`.

## Complexity
`O(N² · M log M + Q log M)`, with `2N` BFS runs per level.

## Build & Run
```bash
g++ -O2 -std=c++17 solution.cpp -o solution
./solution < data/sample/00_main.in
```
