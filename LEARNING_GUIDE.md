# Learning Guide: the DSA behind these solutions

This guide maps every **data structure and algorithm** used in the 11 solutions to the problems that use it. Each problem folder also has a `LEARN.md` with full explanations, analogies, and worked examples, from easy to complex.

> **How to study:** read the problem PDF, try it yourself for 20–30 minutes, then read that problem's `LEARN.md`, then its `solution` file. Finally, code it again from memory.

---

## 1. Concept map

| Concept | Where it's used | Difficulty |
|---|---|---|
| String searching (substring check) | [01 prompt](01_prompt/LEARN.md) | ⭐ |
| Simulation / event-driven thinking / state machines | [02 smoke](02_smoke/LEARN.md) | ⭐ |
| Interactive problems, flushing, worst-case guess bounds | [03 verity](03_verity/LEARN.md) | ⭐ |
| Invariants, hashing / grouping, sorting, binary search | [04 rain](04_rain/LEARN.md) | ⭐⭐ |
| Graph modeling with *states*, Union-Find (DSU) | [05 galaxy](05_galaxy/LEARN.md) | ⭐⭐ |
| Trees and depth, scheduling, greedy + exchange argument | [06 schedule](06_schedule/LEARN.md) | ⭐⭐⭐ |
| XOR algebra, checksums / parity, run-twice problems | [07 forgor](07_forgor/LEARN.md) | ⭐⭐⭐ |
| Offline reverse processing, "next free cell" DSU, integer sqrt | [08 shock](08_shock/LEARN.md) | ⭐⭐⭐ |
| BFS shortest path, divide & conquer on queries | [09 galaxy2](09_galaxy2/LEARN.md) | ⭐⭐⭐⭐ |
| Functional graphs, cycle detection, prefix XOR, binary trie, iterative DFS | [10 dictionary](10_dictionary/LEARN.md) | ⭐⭐⭐⭐ |
| Bitwise greedy, interval reasoning, heaps, parity tricks | [11 matcha](11_matcha/LEARN.md) | ⭐⭐⭐⭐⭐ |

---

## 2. Data structures at a glance

| Data structure | One-line idea | Everyday analogy | Used in |
|---|---|---|---|
| **Array / list** | Items in numbered slots | A row of numbered lockers | all |
| **Hash map** (`dict`, `defaultdict`) | Key → value lookup in O(1) on average | A coat check: give your ticket, get your coat | rain |
| **Sorted list + binary search** | Find positions in O(log n) | Opening a dictionary near the right page, then halving | rain |
| **Queue** (`deque`, array queue) | First in, first out | A line at a ticket counter | forgor, galaxy2 |
| **Stack** | Last in, first out | A pile of plates | dictionary (iterative DFS) |
| **Union-Find (DSU)** | Which group is this item in? Merge two groups. | Friend circles where everyone knows their group leader | galaxy, shock |
| **Binary trie** | A tree that stores numbers bit by bit | A phone menu: "press 0 or 1" at each step | dictionary |
| **Priority queue (heap)** | Always pop the largest or smallest item | An ER triage: the most urgent patient goes first | matcha |

## 3. Algorithms at a glance

| Algorithm / technique | One-line idea | Used in |
|---|---|---|
| Simulation | Do exactly what the statement says, but only at the moments that matter | smoke |
| Greedy | Make the locally best choice, then **prove** it is globally best | schedule, matcha |
| Exchange argument | A way to prove a greedy: show that swapping toward the greedy choice never hurts | schedule |
| BFS | Explore in waves. In unweighted graphs this gives shortest paths. | forgor, galaxy2 |
| DFS (iterative) | Go deep first. Use an explicit stack to avoid recursion limits. | dictionary |
| Divide & conquer | Split the problem, solve the halves, combine | galaxy2 |
| Offline processing | Read all queries first, then answer them in a clever order (e.g. reversed) | shock, galaxy2 |
| Bit manipulation | `^`, `&`, `\|`, `>>`, `<<` used as tools | forgor, dictionary, matcha |

---

## 4. Big-O in 60 seconds

A typical judge does about **10⁸ simple operations per second in C++** and about **10⁷ in Python**. Before coding, estimate the work:

| N | Allowed complexity (about 1 s) |
|---|---|
| ≤ 10 | O(N!) or O(2^N · N) |
| ≤ 20 | O(2^N) |
| ≤ 500 | O(N³) |
| ≤ 5 000 | O(N²) |
| ≤ 10⁶ | O(N log N) or O(N) |

**Example:** in `rain`, comparing every pair of drones is O(N²). With N = 2·10⁵ that is 4·10¹⁰ operations, which is far too slow. Grouping plus binary search gives O(N log N), about 4·10⁶ operations, which is instant.

That's also why problems 8–11 are written in **C++**: they need tens of millions of operations, which is fine in C++ but too slow in Python.

---

## 5. Contest techniques used throughout

### Fast I/O
- **Python:** `sys.stdin.buffer.read().split()` reads everything in one call. Calling `input()` 10⁶ times can take seconds.
- **C++:** one `fread` into a big buffer plus hand-written number parsing, and one `fwrite` at the end. This is much faster than `cin`/`cout` without `sync_with_stdio(false)`.

### Stress testing (how every solution here was verified)
1. Write a **brute-force** solution: slow, but obviously correct (e.g. try every combination).
2. Write a **random generator** for small inputs.
3. Run both programs on thousands of random inputs and compare. The first mismatch gives you a tiny failing test to debug.

> Analogy: checking a new calculator by comparing it with pen-and-paper answers on thousands of small sums.

### Avoid recursion for deep structures
Python's default recursion limit is about 1000, and C++ stacks are small too. Trees and chains can be 2·10⁵ deep, so the solutions use **iterative** loops (DSU `find`, depth computation, DFS with an explicit stack).

---

## 6. Suggested learning path

1. **Basics:** 01 → 02 → 03 (strings, simulation, interaction)
2. **Core techniques:** 04 (binary search) → 05 (DSU) → 07 (XOR)
3. **Proof skills:** 06 (greedy + exchange argument)
4. **Advanced structures:** 08 (DSU tricks) → 10 (tries, functional graphs)
5. **Advanced algorithms:** 09 (divide & conquer + BFS) → 11 (bitwise greedy)
