# Problem 8 — That Was an Earthquake? I Barely Felt It

**Points:** 11 · **Rank:** 3 · **Problem ID:** `shock` · **Solution:** [`solution.cpp`](solution.cpp)

> 📘 **Learning notes:** concepts, analogies and worked examples are in [LEARN.md](LEARN.md). See also the [Learning Guide](../LEARNING_GUIDE.md).

## Problem
An `N × M` grid of seismometers all start at 0. Earthquake `i` at `(R_i, C_i)` with power `P_i` gives each cell the shock level `P_i² − dist² + 1`. A seismometer overwrites its reading only when that level is positive. Output the final grid.

## Approach: reverse order, so each cell is written once
A cell's final value comes from the **last** earthquake that reached it. So process the earthquakes from last to first, and only write cells that are still empty.

- Earthquake `i` affects a disk. For each row `r` in it, the columns are `C_i ± isqrt(P_i² − (r − R_i)²)`.
- To skip cells that are already written, each row keeps a "next unwritten column" union-find (path compression).

Each cell is written at most once, so the total work is about the grid size plus the number of disk rows.

## Code walkthrough
| Part | What it does |
|---|---|
| `readInt()` | Fast input: the whole of stdin is read into `ibuf` with one `fread`, then parsed by hand |
| `nxt[r*W + c]` | Per-row union-find with `W = M + 1`. The root of `c` is the first **unwritten** column `≥ c`. Column `M` is a sentinel that is never written. |
| `findNext(base, c)` | Finds that root, with iterative path compression |
| `isqrtll(x)` | Exact integer square root. The `double` `sqrt` result is corrected by ±1. |

Main loop, for each earthquake `i` from `Q−1` down to `0`:
```cpp
for (dr = -P; dr <= P; dr++) {                  // every row of the disk
    rem = P*P - dr*dr;  w = isqrtll(rem);       // columns C-w .. C+w are shaken
    c = findNext(base, lo);
    while (c <= hi) {                           // only UNWRITTEN cells
        val[r][c] = rem - dc*dc + 1;            // = P^2 - dist^2 + 1
        nxt[base + c] = c + 1;                  // mark written
        c = findNext(base, c + 1);
    }
}
```
Cells never reached by any earthquake keep `0`. Output is built in a `string` and written with `fwrite`, and the buffer is flushed when it grows large.

## Complexity
`O(N·M · α + Σ (2·P_i + 1))` per test case. Input and output use fast `fread`/`fwrite`.

## Build & Run
```bash
g++ -O2 -std=c++17 solution.cpp -o solution
./solution < data/sample/00_main.in
```
