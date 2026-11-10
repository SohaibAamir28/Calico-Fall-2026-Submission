# Learn: Problem 8 — shock (Earthquake)

**Concepts:** offline processing in reverse, "last write wins", the DSU "next free cell" trick, geometry of disks, integer square root, fast I/O · **Difficulty:** ⭐⭐⭐

---

## 1. Last write wins → process in reverse

Each seismometer shows the reading of the **last** earthquake that shook it. Processed forward, a cell can be overwritten many times, and big overlapping quakes cost O(Q · area).

**Flip it:** go through the earthquakes from **last to first**. Now the **first** quake that touches a cell is the final one, so we write it and **never touch that cell again**.

> **Analogy:** painting a wall with many overlapping posters. You only see the top poster at each spot. Instead of gluing them in order, start from the **top** poster, and when placing older posters, only fill spots that are still bare.

This "reverse the order so each item is finalized once" trick shows up often: offline deletions become insertions, painting intervals, and similar problems.

## 2. Which cells does an earthquake touch?

The shock is `P² − dist² + 1 > 0`, which is the same as `dist² ≤ P²`. That's a **disk** of radius P. For each row offset `dr` from `−P` to `P`, the columns are:

```
|dc| ≤ √(P² − dr²)   →   columns  C − w … C + w,   w = isqrt(P² − dr²)
```

### Integer square root, carefully
`sqrt()` uses floating point and can be off by 1 for large numbers (e.g. returning 99.99999 instead of 100). The solution fixes it:
```cpp
w = (long long)sqrt((double)x);
while (w * w > x) w--;
while ((w+1)*(w+1) <= x) w++;
```

## 3. Skipping already-written cells: the "next free" DSU

Inside a row segment `[lo, hi]`, many cells may already be written. Checking them one by one is wasteful. So each row keeps a **union-find over columns**:

- `find(c)` = the **first unwritten column ≥ c**
- After writing column `c`, set `parent[c] = c + 1` ("I'm taken, look to my right")
- An extra **sentinel** column `M` is never written, so `find` always stops

```
row:     0 1 2 3 4 5(sentinel)
written:   ✓ ✓   ✓
find(1) → 1 is taken → 2 is taken → 3 is free → returns 3
(with path compression, 1 and 2 now point straight at 3)
```

> **Analogy:** a cinema where every occupied seat has a sticky note saying "next free seat →". You follow the notes, and you update them as you go so the next person gets there faster.

**Cost:** every cell is written once, and every `find` is almost O(1) (amortized α). Total ≈ **O(N·M + Σ(2P+1))** instead of O(Q · πP²).

## 4. Worked examples

**Easy:** 5×5 grid. Quake A at (2,2) with P = 1, then quake B at (2,3) with P = 1. With P = 1 the center gets `1−0+1 = 2` and the 4 neighbours get `1−1+1 = 1`.

Reverse order, **B first** (writes 5 cells):
```
. . . . .
. . . 1 .
. . 1 2 1
. . . 1 .
. . . . .
```
Then **A** (only empty cells):
- row 1: col 2 is empty → 1
- row 2: cols 1–3. `find(1) = 1` → write 1. `find(2)`: 2 ✓, 3 ✓, 4 ✓ (written by B) → 5 > hi, stop.
- row 3: col 2 → 1

```
. . . . .
. . 1 1 .
. 1 1 2 1
. . 1 1 .
. . . . .
```
Cell (2,2) shows **1** (from B, the later quake), not 2 (from A). ✔

**Complex: why reverse + DSU matters.** Q = 10⁵ quakes, each covering most of a 1000 × 1000 grid. Forward processing writes about 10⁵ × 10⁶ = 10¹¹ cells, which is hopeless. Reverse + DSU writes each of the 10⁶ cells **once**, plus about 2P+1 `find` calls per quake.

## 5. Fast I/O
The output can contain millions of numbers. The solution builds the output in a `std::string` and writes it with one `fwrite`. Printing number by number with `printf` or `cout` is several times slower.

## 6. Common mistakes
- Processing forward with a "skip if already written" check. That gives **first** write wins, which is wrong.
- Floating-point `sqrt` without correction (off-by-one disk edges).
- No sentinel column, so `find` runs past the end of the row.
- Using `int` for `P²` when P is large. Use `long long`.

## 7. Practice
- Search for "DSU painting intervals" or "next pointer DSU". A classic exercise is painting segments with colors in reverse order.
- LeetCode 1562: *Find Latest Group of Size M* (DSU on a line)
