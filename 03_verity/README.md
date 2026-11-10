# Problem 3 — Verity

**Points:** 5 · **Rank:** 1 · **Problem ID:** `verity` · **Solution:** [`solution.py`](solution.py)

> 📘 **Learning notes:** concepts, analogies and worked examples are in [LEARN.md](LEARN.md). See also the [Learning Guide](../LEARNING_GUIDE.md).

## Problem (interactive)
Find a hidden string of length `N` made of `K` allowed characters. You can make up to 100 guesses. After a wrong guess the judge returns the hidden string with every wrong position replaced by `_`. A correct guess gets `CORRECT`.

## Approach
Every position is independent of the others:

- A position the judge has revealed is fixed for good.
- Every other position tries the next unused allowed letter.

Each wrong guess removes one candidate letter at every unknown position, so the answer is found within **`K ≤ 26` guesses**, well under the limit of 100. Each guess is printed with `flush=True`.

## Code walkthrough
| Variable / function | Meaning |
|---|---|
| `known[i]` | The confirmed letter at position `i`, or `None` |
| `ptr[i]` | Index into `A` of the letter to try next at position `i` |
| `print_guess(g)` | Prints the guess with `flush=True`, which every interactive problem needs |
| `solve(N, K, A)` | The guess loop for one test case |

The loop inside `solve`:
1. Build the guess: use `known[i]` if it is set, otherwise `A[ptr[i]]`.
2. Print it and read the response. Stop on `CORRECT`.
3. For each unknown position, `resp[i] != "_"` means the letter was right, so store it. Otherwise increase `ptr[i]`.

`main()` skips blank lines before reading `N K`, so extra blank lines from the judge don't break it.

## Complexity
`O(N · K)` per test case.

## Testing
Tested with a local interactive judge on 100 random cases. The most guesses any case needed was 26.
