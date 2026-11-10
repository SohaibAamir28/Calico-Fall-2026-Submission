# Problem 11 — A Matcha a Day

**Points:** 15 · **Rank:** 4 · **Problem ID:** `matcha` · **Solution:** [`solution.cpp`](solution.cpp)

> 📘 **Learning notes:** concepts, analogies and worked examples are in [LEARN.md](LEARN.md). See also the [Learning Guide](../LEARNING_GUIDE.md).

## Problem
Choose `f_i ∈ [L_i, R_i]` for each of `N` days to maximize `f_1 ⊕ f_2 ⊕ … ⊕ f_N`.

## Approach: greedy from the highest bit
Go from bit 29 down to bit 0 and keep a list of ranges below `2^(b+1)` plus an XOR offset `c`. At bit `b`, sort every range into one of three kinds:

| Kind | Bit `b` of the chosen value |
|---|---|
| Entirely below `2^b` | 0 |
| Entirely at or above `2^b` | 1 (strip the bit) |
| **Spanning** (`L < 2^b ≤ R`) | either |

- **No spanning range:** bit `b` of the answer is forced.
- **At least one spanning range:** bit `b = 1` is always achievable and beats any choice that gives 0. This fixes the parity of how many spanning ranges take their upper half. For a spanning range:
  - Taking the upper half leaves the prefix range `[0, R − 2^b]` for the lower bits.
  - Taking the lower half leaves `[L, mask]`, which is `mask ⊕ [0, mask − L]`. The `mask` part goes into `c`.

**Collapsing the spanning ranges.** The XOR of several prefix ranges `[0, x_i]` is again a prefix range `[0, z]`, and `zMerge` computes `z` greedily. A bigger `z` gives a superset of values, so it is always at least as good. That means all spanning ranges can be replaced by **one** range `[0, Z]`. To get `Z`:

- Every range takes its larger option.
- If that gives the wrong parity, exactly one range `k` takes its smaller option, and we take the best `k`.
- `z` depends only on the ~31 largest values. So when there are more than 40 spanning ranges, fixing the parity costs nothing.

## Code walkthrough
**`zMerge(v)`**: the largest XOR you can make from `y_i ∈ [0, v_i]`. The reachable set is exactly `[0, result]`.
- Repeatedly take the largest value `x` and let `h` be its top bit.
- If a second value also reaches `2^h`, the two of them can make every number below `2^(h+1)`, so the result gets all bits up to `h` set and the loop stops.
- Otherwise set bit `h` and put `x − 2^h` back.
- Only the 40 largest values can matter, so it keeps just those (`nth_element`).

**Main loop**, for `b = 29 … 0`, with `half = 2^b` and `mask = half − 1`:

| Variable | Meaning |
|---|---|
| `cur` / `nxt` | The ranges for this bit / the next bit |
| `parity` | XOR of bit `b` over the ranges that are forced to 1 |
| `optA[i]`, `optB[i]` | For a spanning range: the upper-half option `R − half`, and the lower-half option `mask − L` (with `mask` added to `c`) |
| `c` | XOR offset that the lower bits must cancel |
| `pUp` | Required parity of the number of spanning ranges that pick the upper half, so that bit `b` of the answer is 1 |
| `lowPar` | Parity of the number of lower-half picks. Each one XORs `mask` into `c`. |

- **No spanning range:** bit `b = cb ^ parity`, nothing to choose.
- **One spanning range:** its side is forced by `pUp`.
- **Several spanning ranges:**
  - Everyone takes `Mx = max(optA, optB)`, and that gives `Z = zMerge(Mx)` when the parity already works out (or a tie, or more than 40 ranges, makes it free).
  - Otherwise try switching each `k` to `Mn[k]` and keep the best `Z`.
- The spanning ranges are replaced by the single range `{0, Z}`.

## Complexity
About `O(30 · N log N)` in the worst case. A single test with `N = 2·10^5` runs in about 0.04 s.

## Testing
Cross-checked against an exhaustive brute force on 7 500 random small cases, plus 4 000 extra cases with `N ≤ 9`.

## Build & Run
```bash
g++ -O2 -std=c++17 solution.cpp -o solution
./solution < data/sample/00_main.in
```
