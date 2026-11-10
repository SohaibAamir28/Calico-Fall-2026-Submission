# Learn: Problem 9 — galaxy2 (Super Ben Galaxy 2)

**Concepts:** BFS shortest paths on a state graph, separators, divide & conquer over queries (offline), exploiting small constraints · **Difficulty:** ⭐⭐⭐⭐

> Read [Problem 5's LEARN.md](../05_galaxy/LEARN.md) first. This problem uses the same state graph, but now every step and spin costs 1 second, and we need **shortest** distances, not just connectivity.

---

## 1. BFS = shortest paths when every edge costs 1

BFS explores in **waves**: first everything at distance 1, then distance 2, and so on. The first time it reaches a node, that is the shortest distance.

```
dist 0:  S
dist 1:  neighbours of S
dist 2:  their new neighbours ...
```

> **Analogy:** drop a stone in a pond. The ripple reaches closer points first. The moment the ripple touches a point is its shortest distance.

DSU can't help here, because it only knows **whether** two states are connected, not **how far apart** they are.

## 2. The problem: too many queries

- One BFS per query: O(Q · N · M), too slow.
- All-pairs distances: (2NM)² values, way too much memory.

**Use the small constraint:** N ≤ 10 rows. The grid is **long and thin**.

## 3. Separators: every path must cross the middle

Split the columns at `mid`. If the start is left of `mid` and the end is right of it, **every** path between them goes through some state in column `mid`. There are at most `2N ≤ 20` such states. So:

```
dist(start, end) = min over states s in column mid of  dist(start, s) + dist(s, end)
```

We run **one BFS from each of the ≤ 20 middle states**, and that answers *every* query crossing `mid`.

> **Analogy:** a city split by a river with only 20 bridges. Any trip from the west bank to the east bank uses one bridge. Measure every location's distance to each bridge once, and any cross-river trip is "min over bridges of (to bridge + from bridge)".

## 4. Divide & conquer on columns

What about queries entirely on the left of `mid`? Recurse on the left half, and do the same for the right.

```
                 [0..6] mid=3
               /             \
        [0..2] mid=1      [4..6] mid=5
        /      \           /      \
     [0..0]  [2..2]     [4..4]  [6..6]
```

**Subtle point:** a query with both ends on the left may still have its shortest path **detouring through the right side**, e.g. around a wall. That's why each query is evaluated **at every node on its way down**, not only at the last one.

**Correctness:** take the deepest node whose column range contains the entire shortest path. That path must cross that node's `mid` column (otherwise it would fit inside a child), and the BFS restricted to that range is exact for it. The query passes through that node, so it gets the right value.

## 5. Complexity
- Each recursion level covers M columns in total. Each node runs ≤ 2N BFS runs over `width × N × 2` states, so a level costs **O(N² · M)**.
- There are **log M** levels → O(N² · M log M).
- Each query visits ≤ log M nodes and checks ≤ 2N sources at each one.

## 6. Worked examples

**Easy (spin costs time):**
```
. R .
```
Start (0,0) in blue mode, end (0,2):
- Spin → (0,0, red): 1 s
- Walk → (0,1) R: 2 s
- Walk → (0,2): 3 s

The distance is **3**, so `K = 2` → NO and `K = 3` → YES.

**Complex (a detour):**
```
. R . . .
. R . . .
. . . . .
```
Start (0,0), end (0,2), in blue mode:
- Spin and cross the R tiles: 1 + 1 + 1 = **3**
- Walk around the bottom in blue mode: (1,0) (2,0) (2,1) (2,2) (1,2) (0,2) = **6**

BFS finds 3. If the R column were longer and blocked the bottom row, the only way might be a long detour, possibly far to the right. That is why queries are checked at every ancestor separator.

## 7. Implementation tips from the code
- **Local state ids** `((col − l) * N + row) * 2 + mode` make each BFS array exactly as big as the segment.
- **Reused buffers** (`distBuf`, `bfsQ`) avoid allocating memory again for every node.
- Using an **array as the queue** (`head`/`tail` indices) is faster than `std::queue`.
- Since the graph is undirected, `dist(s, start) = dist(start, s)`, so one BFS from `s` gives both halves of the sum.

## 8. Related ideas
- **Centroid decomposition** (on trees) and **planar separators** use the same "everything crosses a small separator" principle.
- **Meet in the middle** also splits a problem and combines through the middle.

## 9. Practice
- LeetCode 1293: *Shortest Path in a Grid with Obstacles Elimination* (BFS on states)
- LeetCode 864: *Shortest Path to Get All Keys*
- CSES: *Labyrinth* (BFS with path reconstruction)
