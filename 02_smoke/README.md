# Problem 2 — Valorant

**Points:** 4 + 3 (main + bonus) · **Rank:** 1 + 2 · **Problem ID:** `smoke` · **Solution:** [`solution.py`](solution.py)

> 📘 **Learning notes:** concepts, analogies and worked examples are in [LEARN.md](LEARN.md). See also the [Learning Guide](../LEARNING_GUIDE.md).

## Problem
Omen starts with 2 smoke charges (the maximum). Using a smoke uses one charge, and a charge takes `R` seconds to come back. Charges recharge one at a time: if both are used, the second one starts recharging only after the first is restored. Given `N` requests at increasing timestamps `A_i`, decide whether every request can be fulfilled. Recharges at time `t` happen before a request at time `t`.

## Approach
Simulate only at request times and track two values:

- `charges`: how many charges are available now
- `ready_at`: when the charge that is currently recharging will be restored

At each request:
1. Apply finished recharges: while `charges < 2` and `ready_at <= t`, add a charge. If another charge is still missing, its recharge starts right away, so `ready_at += R`.
2. If `charges == 0`, the answer is `NO`.
3. If `charges == 2`, nothing was recharging, so recharging starts now: `ready_at = t + R`.
4. Use one charge.

## Code walkthrough
```python
charges = 2      # available charges
ready_at = 0     # when the recharging charge comes back
for t in A:
    while charges < 2 and ready_at <= t:   # (1) apply finished recharges
        charges += 1
        if charges < 2: ready_at += R      #     next recharge starts immediately
    if charges == 0: return "NO"           # (2) nothing to use
    if charges == 2: ready_at = t + R      # (3) start a fresh recharge timer
    charges -= 1                           # (4) use a smoke
```
- The `while` loop runs at most twice per request, because at most 2 charges can come back.
- `ready_at` only matters while `charges < 2`. When we are full and use a charge, the timer restarts at `t + R`.
- `main()` reads all tokens at once with `sys.stdin.buffer.read().split()`, which is faster than `input()` for large inputs.

## Complexity
`O(N)` per test case. This passes both the main and the bonus constraints.

## Note
The explanation of sample 3 in the PDF says a recharge finishes at second 22. Under the stated rules (sequential recharge starting the moment the first charge is restored), this solution gives 21. All samples and a brute-force cross-check pass either way.

## Run
```bash
python solution.py < data/sample/00_main.in
```
