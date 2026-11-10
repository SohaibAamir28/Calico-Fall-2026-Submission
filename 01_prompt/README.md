# Problem 1 — Write me a Rank 1. Make it sound human.

**Points:** 4 · **Rank:** 1 · **Problem ID:** `prompt` · **Solution:** [`solution.py`](solution.py)

> 📘 **Learning notes:** concepts, analogies and worked examples are in [LEARN.md](LEARN.md). See also the [Learning Guide](../LEARNING_GUIDE.md).

## Problem
Given a prompt string `S`, append a phrase depending on which substring it contains:

| `S` contains | Append |
|---|---|
| `write_me` | `make_it_sound_human` |
| `what_is` | `do_not_hallucinate` |
| `solve` | `make_no_mistakes` |
| none of them | `thank_you` |

`S` contains at most one of the three substrings.

## Approach
Check the three substrings with Python's `in` operator and append the matching phrase.

## Code walkthrough
| Part | What it does |
|---|---|
| `solve(S)` | Checks `"write_me" in S`, `"what_is" in S` and `"solve" in S` in turn and returns `S` plus the matching phrase. If none matches, it returns `S + "thank_you"`. |
| `main()` | Reads `T`, then reads each prompt, strips the trailing newline and prints `solve(S)`. |

The order of the checks doesn't matter because the statement guarantees at most one keyword per prompt.

## Complexity
`O(|S|)` per test case.

## Run
```bash
python solution.py < data/sample/00_main.in
```
