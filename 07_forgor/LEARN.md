# Learn: Problem 7 — forgor (Big Ben Forgor)

**Concepts:** XOR algebra, checksums / parity, erasure recovery, tree encoding with a parent array, BFS, run-twice problems · **Difficulty:** ⭐⭐⭐

---

## 1. What is a "run-twice" problem?

Your program runs **two separate times**:
1. **Phase 1:** it sees the full data and must write a short "note".
2. **Phase 2:** it remembers nothing (all variables are gone) and gets the note back **with one number erased**. It must rebuild the full data.

> **Analogy:** before an exam you write a cheat-sheet, but someone spills coffee on one number. The sheet has to be designed so that you can still recover everything.

## 2. Encoding a tree in N numbers: the parent array

Root the tree at vertex 1 and record each vertex's **parent**. That takes N−1 numbers and fully describes the tree:

```
edges: 1-2, 1-3, 3-4

      1          parent(2) = 1
     / \         parent(3) = 1
    2   3        parent(4) = 3
        |
        4
```

To find the parents we run a **BFS** from vertex 1. Every newly discovered neighbour gets the current vertex as its parent.

We are allowed **N** numbers but need only **N−1**, so one slot is free. It's used for a **checksum**.

## 3. XOR: the perfect checksum

XOR (`^`) works bit by bit: 1 if the bits differ, 0 if they're the same.

| Property | Formula | Why it matters |
|---|---|---|
| Identity | `a ^ 0 = a` | Starting value 0 changes nothing |
| Self-inverse | `a ^ a = 0` | A value cancels itself |
| Commutative & associative | order and grouping don't matter | We can XOR everything in any order |

So if `S = a ^ b ^ c ^ d` and `c` is lost:

```
S ^ a ^ b ^ d = (a^a) ^ (b^b) ^ c ^ (d^d) = c
```

> **Analogy:** **RAID 5** in data centers. Several disks hold data and one extra disk holds the XOR of all of them. If any **one** disk dies, its contents are rebuilt by XORing the survivors. This problem is exactly RAID 5 on a list of parents.

## 4. Why XOR and not a sum?

A sum checksum also works mathematically: `missing = total − (sum of the others)`. But the output values must be **≤ 10⁶**.
- Sum of N−1 parents: up to about (2·10⁵)² = 4·10¹⁰, which is too big ❌
- XOR of numbers `< 2^18 = 262 144`: always `< 2^18` ✅ (XOR never creates a higher bit than its inputs have)

## 5. Worked examples

**Easy:** edges 1-2, 1-3, 3-4

| Phase 1 | Value |
|---|---|
| parents (2, 3, 4) | 1, 1, 3 |
| checksum = 1 ^ 1 ^ 3 | 3 |
| output | `3 1 1 3` |

Phase 2, the input is `3 1 -1 3` (the parent of 3 was erased):
```
missing = 3 ^ 1 ^ 3 = 1     → parent(3) = 1 ✔
```

**Edge case:** the input is `-1 1 1 3` (the checksum itself was erased). All the parents are still there, so just output the edges.

**Complex: why it always works.** Exactly one value is missing, and the checksum is one equation with one unknown. XOR is a bijection (for fixed `y`, `x → x ^ y` is one-to-one), so the unknown is determined uniquely. This is the simplest **erasure-correcting code**: one parity value can repair one known missing position.

## 6. Going further
- To survive **two** erasures you need more redundancy, e.g. **Reed–Solomon** codes (used on CDs, QR codes and RAID 6).
- This problem is a "communication" problem. Others ask you to encode data into as few bits as possible, or to send messages between two runs.

## 7. Practice
- LeetCode 136: *Single Number* (XOR cancels the pairs)
- LeetCode 268: *Missing Number* (XOR checksum, almost the same idea)
- LeetCode 1720: *Decode XORed Array*
