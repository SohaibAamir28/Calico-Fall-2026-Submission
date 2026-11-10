# Learn: Problem 3 — verity (interactive)

**Concepts:** interactive problems, I/O flushing, independence, worst-case (adversary) analysis · **Difficulty:** ⭐

---

## 1. What is an interactive problem?

Normally your program reads all of the input and prints all of the output. In an **interactive** problem your program **talks back and forth** with the judge:

```
judge → you : N=3, letters A B C
you → judge : AAA
judge → you : _A_
you → judge : BAB
judge → you : _AB
you → judge : CAB
judge → you : CORRECT
```

### The #1 rule: **flush** your output
Programs keep printed text in a buffer and send it later. If you don't flush, the judge waits for your guess while you wait for its answer, and both sides hang until the time limit.

| Language | How to flush |
|---|---|
| Python | `print(x, flush=True)` |
| C++ | `cout << x << endl;` (`endl` flushes) or `fflush(stdout)` |
| Java | `System.out.flush()` |

> **Analogy:** a walkie-talkie. You must say **"over"** (flush) or the other person keeps waiting.

## 2. Key insight: positions are independent

The judge's feedback is **per position**: "your letter at position *i* is right or wrong". It says nothing about other positions. So we can solve **N separate tiny problems at the same time**, one per position, all within the same guesses.

At each position we try the allowed letters in order. Every wrong guess removes one candidate **at every unknown position at once**.

> **Analogy:** a combination lock with N dials, where the lock tells you which dials are correct. You turn every wrong dial one notch each round, all together.

## 3. How many guesses?

At one position, after `j` wrong tries, `j` letters are ruled out. With `K` letters the right one is found by guess number **K** at the latest. Since every position runs in parallel:

```
total guesses ≤ K ≤ 26  (the limit is 100)
```

**Adversary view (why we can't guarantee fewer):** imagine the judge picks the hidden letter *after* seeing your guesses, always the last one you would try. At a single position each guess tests only one letter, so the judge can force K guesses. Our strategy matches this lower bound, so it is optimal in the worst case.

## 4. Worked examples

**Easy:** N = 3, letters `A B C`, hidden `CAB`

| Guess | Response | What we learn |
|---|---|---|
| `AAA` | `_A_` | pos 1 = A ✔; pos 0, 2 move to the next letter |
| `BAB` | `_AB` | pos 2 = B ✔; pos 0 moves to the next letter |
| `CAB` | `CORRECT` | done in 3 = K guesses |

**Complex:** N = 100, K = 26, hidden string uses all letters. Every guess fixes the positions whose current letter is right. Position *i* is fixed after `(index of hidden[i] in A) + 1` guesses, so the slowest position decides the total, which is at most 26.

## 5. Code anatomy

```python
known = [None] * N   # confirmed letters
ptr   = [0] * N      # next letter index to try at each position
```
Each round builds the guess from `known`/`ptr`, flushes it, and reads the response. On `_` it does `ptr[i] += 1`.

## 6. Common mistakes
- Forgetting to flush (Time Limit Exceeded or "Idleness limit").
- Reading too much input at once with `sys.stdin.read()`. In an interactive problem that **blocks**, because the judge hasn't sent the rest yet. Read line by line.
- Not handling `CORRECT` before indexing into the response.

## 7. Practice
- LeetCode 374: *Guess Number Higher or Lower* (binary search, interactive style).
- Codeforces has an "interactive" tag. Try any problem rated around 1200 with it.
