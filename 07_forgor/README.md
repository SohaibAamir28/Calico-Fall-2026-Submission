# Problem 7 — Big Ben Forgor

**Points:** 10 · **Rank:** 3 · **Problem ID:** `forgor` · **Solution:** [`solution.py`](solution.py)

> 📘 **Learning notes:** concepts, analogies and worked examples are in [LEARN.md](LEARN.md). See also the [Learning Guide](../LEARNING_GUIDE.md).

## Problem (run-twice)
- **Phase 1:** given a tree on `N` vertices, output a list of `N` integers `A_1 … A_N`.
- **Phase 2:** you get that list back with **one** value replaced by `-1`, and must rebuild the whole tree.

## Approach: parent array plus an XOR checksum
**Phase 1.** Root the tree at vertex 1 with a BFS.
- For `i ≥ 2`, output `A_i = parent(i)`.
- Output `A_1 = parent(2) ⊕ parent(3) ⊕ … ⊕ parent(N)`. It fits in the value limit because every parent is below `2^18 ≤ 10^6`.

**Phase 2.**
- If `A_1` was erased, every parent is still known, so the tree can be rebuilt directly.
- Otherwise the erased parent is `A_1 ⊕ (XOR of all the other parents)`.

Then output the edges `(i, A_i)` for `i = 2 … N`.

## Code walkthrough
| Function | What it does |
|---|---|
| `solve_phase1(N, E)` | Builds an adjacency list and runs a BFS from vertex 1 to fill `par[]`. It XORs `par[2..N]` into `x` and returns `[x, par[2], …, par[N]]`. |
| `solve_phase2(N, A)` | If `A[0]` (the checksum) survived, it finds the `-1` with `A.index(-1)` and rebuilds that value as `A[0] ^ (XOR of every other A[i])`, since `x ^ x = 0` cancels each known parent. Then it returns the edges `(i+1, A[i])`. |
| `main()` | Reads the first token (`FIRST` or `SECOND`) and calls the matching phase. |

Why the checksum fits: every parent is a vertex number `≤ N < 2^18`, so a XOR of them is also `< 2^18 ≤ 10^6`.

## Complexity
`O(N)` in each phase.

## Testing
Tested with a two-phase local harness. For small random trees it erased every possible position, and it also ran on a tree with `N = 200 000`.
