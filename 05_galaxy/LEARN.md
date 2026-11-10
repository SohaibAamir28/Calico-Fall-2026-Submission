# Learn: Problem 5 — galaxy (Super Ben Galaxy)

**Concepts:** graph modeling, state-space graphs, connectivity, Union-Find (DSU) · **Difficulty:** ⭐⭐

---

## 1. Graph modeling with *states*

A tile alone doesn't describe where Ben is. We also need his **mode** (red or blue), because the mode decides which tiles he can step on. So a node of our graph is a **state**:

```
state = (row, col, mode)      → 2 · N · M states in total
```

Edges:
- **Walk:** `(r, c, m) — (r', c', m)` if both tiles are active in mode `m`
- **Spin:** `(r, c, blue) — (r, c, red)` if the tile is normal (`.`)

> **Analogy:** a building with **two floors**, a blue floor and a red floor. Each tile exists on both floors, but red tiles are holes on the blue floor and blue tiles are holes on the red floor. Normal tiles have a **staircase** between floors (the spin). The question becomes: can I walk from my room to the target room, using stairs as needed?

This "add extra information to the node" trick is called a **state-space graph** (or **layered graph**). It solves many puzzles: keys and doors, limited fuel, "you may break one wall", and so on.

## 2. Reachability = connectivity (because moves are reversible)

Every move can be undone: walk back, or spin again. So the graph is **undirected**, and "can I reach B from A?" is the same as "are A and B in the same **connected component**?"

Options:
| Approach | Cost | Verdict |
|---|---|---|
| BFS per query | O(Q · N · M) | too slow for many queries |
| Label components once (BFS/DFS), then compare labels | O(N·M + Q) | ✅ works |
| **Union-Find** while scanning the grid | O((N·M + Q) · α) | ✅ what we use, very simple to code |

## 3. Union-Find (Disjoint Set Union) explained

DSU maintains groups of items with two operations:
- `find(x)`: which group (its **root/leader**) is `x` in?
- `union(a, b)`: merge the groups of `a` and `b`.

Each item stores a `parent`. A root is its own parent.

```
Initially: 0  1  2  3  4      (everyone alone)
union(0,1): 1←0               union(2,3): 3←2
union(1,3): 3←1←0, 3←2        find(0) → 1 → 3 = root
```

### Path compression
After `find(0)` walks `0 → 1 → 3`, we point `0` **directly** at `3`. Later finds are almost instant.

> **Analogy:** a company org chart. To find the CEO you ask your manager, who asks theirs… With path compression, once you learn who the CEO is, you write it down, and next time you go straight there.

With path compression (and optionally union by size), each operation costs **α(n)**, the inverse Ackermann function. It is ≤ 4 for any realistic n, so it is effectively constant.

The solution's `find` is **iterative** (two loops: find the root, then re-point everything). A recursive `find` could exceed Python's recursion limit on long chains.

## 4. Worked examples

**Easy:** `1 × 3` grid `. R .`, start (0,0), target (0,2)
- Start: (0,0, blue). R is a hole on the blue floor.
- Spin at (0,0): now (0,0, red) → walk to (0,1) (R is active in red) → walk to (0,2) (normal). **YES**

**Blocked:** `1 × 4` grid `. R B .`, start (0,0), target (0,3)
- Red mode reaches (0,1). To cross B you need blue mode, but you can only spin on a normal tile, and (0,1) is R. **NO**

**Complex: the union-find in action**
For `. R .` the DSU unions are:
1. (0,0): `.` → union(blue, red) (spin)
2. (0,0) red — (0,1) red (R active in red) → union
3. (0,1) red — (0,2) red → union
4. (0,2): `.` → union(blue, red)

All of these states end in **one** component, so any query between them is YES.

## 5. Common mistakes
- Treating the grid as tiles only and ignoring the mode, which gives wrong answers.
- Forgetting that the destination may be reached in **either** mode (whichever is active there).
- Recursive DSU `find` in Python on 10⁶ states can crash with a RecursionError.

## 6. Practice
- LeetCode 200: *Number of Islands* (connectivity on a grid)
- LeetCode 547: *Number of Provinces* and 684: *Redundant Connection* (DSU basics)
- LeetCode 864: *Shortest Path to Get All Keys* (state-space graph)
- CSES: *Building Roads*
