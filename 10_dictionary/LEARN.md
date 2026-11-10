# Learn: Problem 10 — dictionary

**Concepts:** functional graphs, cycle detection, prefix XOR, binary trie (max XOR), DFS with add/remove (backtracking), iterative DFS · **Difficulty:** ⭐⭐⭐⭐

---

## 1. Functional graphs: every node has exactly one outgoing edge

Word `i` → word `A_i`. Every node points to exactly one node. Start anywhere and keep following: you can't go on forever without repeating (there are only N nodes), so you eventually fall into a **cycle**. The walk looks like the Greek letter **ρ (rho)**:

```
 start → → → ┐
             ↓
        ┌ ← ← ┐
        ↓     ↑     (cycle, repeats forever)
        └ → → ┘
```

Every functional graph is a set of **cycles**, each with **trees hanging into it** (the "tails").

> **Analogy:** a dictionary where each definition says "see also: X". Keep following the "see also" links and you'll eventually loop back to a word you've already read.

## 2. Finding the cycles (three-color walk)

Each node is in one of three states: `0` = unvisited, `1` = on the current walk, `2` = finished. Walk from an unvisited node, marking nodes `1`.
- If you hit a node marked `1`, you just **closed a new cycle**. Mark it.
- If you hit a node marked `2`, you joined something already explored.
- Afterwards, mark the whole walk `2`.

Each node is walked once, so this is O(N).

**Alternative:** Floyd's **tortoise and hare** (two pointers, one twice as fast) finds a cycle with O(1) memory. It is famous for linked-list cycle detection.

## 3. Prefix XOR: the XOR along any path in O(1)

Like prefix sums: if `P[k] = V_0 ^ V_1 ^ … ^ V_k`, then the XOR of the segment `V_{i+1} … V_j` is `P[j] ^ P[i]`, because XOR cancels the common part.

In the trees we define `W(u)` = the XOR from `u` up to (not including) the cycle node. Then the XOR of the path from `u` up to (not including) an ancestor `v` is just **`W(u) ^ W(v)`**.

On the cycle, with prefix values `Z_j` and whole-cycle XOR `C`:
- One more full lap XORs `C` in once, and two laps cancel (`C ^ C = 0`).
- So, measured from `c_0`, the reachable values are exactly `{Z_e, Z_e ^ C}` for every `e`.
- Starting at `c_s` just XORs `Z_{s−1}` in front.

## 4. Binary trie: "which number in my set maximizes x ^ y?"

Store numbers as 30-bit binary strings in a tree: from the root, branch 0 or 1 for bit 29, then bit 28, and so on.

To maximize `x ^ y`, go greedily from the top bit and **prefer the opposite bit of x** at each step, since a 1 in a higher bit beats all lower bits combined.

```
x = 0b101. Set = {0b001, 0b110, 0b011}
bit 2: x has 1 → want 0 → exists (001, 011) ✓ result 1__
bit 1: x has 0 → want 1 → 011 ✓              result 11_
bit 0: x has 1 → want 0 → (011 has 1) ✗ take 1 → result 110
→ best y = 011, x ^ y = 110 = 6
```

Check: 101^001 = 100 (4), 101^110 = 011 (3), 101^011 = 110 (6) ✔

> **Analogy:** a "20 questions" game, or a phone menu where you press 0 or 1 at each level. At each fork you take the road **opposite** to x if it exists.

**Deletion:** each node keeps a `cnt`. Removing a number decrements counts along its path, and a branch with `cnt = 0` counts as empty.

## 5. DFS with add/remove (ancestor set)

For a tree node `u`, we need max-XOR over the `W` values of **its ancestors only**. A DFS gives exactly that:
- **Entering** `u`: query the trie, then insert `W(u)`.
- **Leaving** `u`: remove `W(u)`.

At any moment, the trie holds exactly the current root-to-node path. This is the **backtracking** pattern.

**Iterative DFS:** a chain of 2·10⁵ nodes would overflow the call stack with recursion. The code uses an explicit stack and pushes `-u` as a marker for "leave u later".

## 6. Worked examples

**Easy:** N = 3, `A = [2, 3, 2]`, `V = [1, 2, 4]`
- Graph: 1 → 2 → 3 → 2 … The cycle is {2, 3}, and node 1 is a tail.
- From 1, the prefixes are 1, 1^2 = 3, 3^4 = 7, 7^2 = 5, 5^4 = 1, … so the max is **7**.
- From 2, the prefixes are 2, 6, 4, 0, … so the max is **6**. From 3: 4, 6, 2, 0, … max **6**.

**How the algorithm gets it:**
- Cycle `c_0 = 2, c_1 = 3`: `Z = [2, 6]`, `C = 6`, and the trie holds {2, 4, 6, 0}.
- `ans[2] = maxXor(0) = 6` ✔. `ans[3] = maxXor(Z_0 = 2)` = max(0, 6, 4, 2) = 6 ✔
- Node 1: `W(1) = 1`. Ancestor trie {0} → 1. Cycle trie: maxXor(1) = 1^6 = **7** ✔

**Complex: why each piece is needed**
- Without the **tail trie**, you'd miss stopping in the middle of a tail. Take `A = [2, 3, 3]`, `V = [8, 8, 0]`: node 1 → 2 → 3, and 3 points to itself with value 0. From node 1, every prefix that reaches the cycle XORs to `8 ^ 8 = 0`, but stopping right after word 1 gives **8**. The solution outputs `8 8 0`. ✔
- Without the **`Z ^ C`** values, you'd miss answers that need an extra lap.

## 7. Complexity
Each node does a constant number of trie operations of 30 steps each: **O(30 · N)**.

## 8. Practice
- LeetCode 141 / 142: *Linked List Cycle* (Floyd)
- LeetCode 421: *Maximum XOR of Two Numbers in an Array* (binary trie)
- LeetCode 1707: *Maximum XOR With an Element From Array* (trie + offline queries)
- CSES: *Planets Cycles*, *Planets Queries I* (functional graphs)
