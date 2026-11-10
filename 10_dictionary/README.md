# Problem 10 — dictionary

**Points:** 13 · **Rank:** 4 · **Problem ID:** `dictionary` · **Solution:** [`solution.cpp`](solution.cpp)

> 📘 **Learning notes:** concepts, analogies and worked examples are in [LEARN.md](LEARN.md). See also the [Learning Guide](../LEARNING_GUIDE.md).

## Problem
Word `i` points to word `A_i` and has a value `V_i`. Starting from word `i`, Big Ben follows the pointers, XOR-ing each value into his happiness `h`, and can stop at any point. For every `i`, find the maximum `h` he can reach.

## Approach
`i → A_i` is a **functional graph**. From any start, the walk is a tail that leads into a cycle and then loops around that cycle forever. The possible values of `h` are the XORs of the non-empty prefixes of that walk.

- **Tail part.** Let `W(u)` be the XOR along the path from `u` up to its cycle node, not including the cycle node. The tail prefixes from `u` are `W(u) ⊕ W(v)` for every ancestor `v` on the way to the cycle.
- **Cycle part.** Number the cycle nodes `c_0 … c_{L−1}` and let `Z_j = V_{c_0} ⊕ … ⊕ V_{c_j}`, with `C` the XOR of the whole cycle. Going around the cycle a second time XORs in `C` again, so the reachable values only depend on the parity of the number of loops. Entering the cycle at `c_s`, they are `W(u) ⊕ Z_{s−1} ⊕ y` with `y ∈ {Z_e, Z_e ⊕ C}`.

Both parts are **max-XOR queries**, answered with binary tries:
- One static trie per cycle holding every `Z_j` and `Z_j ⊕ C`.
- One trie with insert and erase that holds the `W` values of the current DFS ancestors while walking the in-trees.

## Code walkthrough
**`struct Trie`**: a binary trie over 30-bit values, stored as arrays (`c0`, `c1`, `cnt`).
- `add(x, +1)` inserts `x` and `add(x, −1)` removes it. Removal only decreases the counts.
- `maxXor(x)` walks from the top bit and takes the child with the opposite bit whenever its `cnt > 0`.

**Step 1: find the cycles.** Walk from every unvisited node, marking nodes `state = 1` while they are on the current walk. Reaching a node with `state == 1` means a new cycle was closed, so mark every node on it with `onCycle`.

**Step 2: reverse the tree edges.** Each non-cycle node `i` is added to the child list of `A[i]` (`head` / `nxtE` form a linked list).

**Step 3: handle each cycle.**
- Store the cycle as `c_0 … c_{L−1}` and its prefix XORs as `Z[j]`, with `Ctot` the XOR of the whole cycle.
- `cyc` holds every `Z[j]` and `Z[j] ^ Ctot`. These are all the values a walk starting at `c_0` can stop at.
- A walk that starts at `c_s` gives those values with `pre = Z[s−1]` XORed in. So `ans[c_s] = cyc.maxXor(pre)`.
- An iterative DFS (using an explicit stack, with a negative marker for "leaving a node") runs over the tree hanging from `c_s`. For each node `u`:
  ```cpp
  Wv[u]  = V[u] ^ Wv[A[u]];                 // XOR from u up to the cycle
  best   = anc.maxXor(Wv[u]);               // stop somewhere on the tail
  viaCyc = cyc.maxXor(Wv[u] ^ pre);         // stop somewhere on the cycle
  ans[u] = max(best, viaCyc);
  ```
  `anc` holds the `W` values of all ancestors of `u`. That includes `0` for the cycle node itself, which covers stopping right after `u`'s own tail.

## Complexity
`O(N · 30)` per test case.

## Build & Run
```bash
g++ -O2 -std=c++17 solution.cpp -o solution
./solution < data/sample/00_main.in
```
