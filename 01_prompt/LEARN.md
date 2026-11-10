# Learn: Problem 1 — prompt

**Concepts:** strings, substring search, conditional logic · **Difficulty:** ⭐

---

## 1. Substring vs. subsequence

A **substring** is a *contiguous* block of characters. A **subsequence** keeps the order but may skip characters.

| Text | Pattern `solve` | Substring? | Subsequence? |
|---|---|---|---|
| `how_to_solve_dp` | `solve` | ✅ | ✅ |
| `s_o_l_v_e` | `solve` | ❌ | ✅ |
| `evlos` | `solve` | ❌ | ❌ |

This problem asks about **substrings**, which is exactly what Python's `in` operator checks.

> **Analogy:** a substring is a word you can find with **Ctrl+F**. A subsequence is like spotting letters scattered across a page that spell the word in order.

## 2. How does `pattern in text` work?

**Naive algorithm:** try every starting position and compare character by character.

```
text:    h o w _ t o _ s o l v e _ d p
pattern:               s o l v e
                       ↑ match starts at index 7
```

```python
def contains(text, pat):
    for i in range(len(text) - len(pat) + 1):   # every start
        if text[i:i + len(pat)] == pat:          # compare
            return True
    return False
```

- Worst case: **O(n · m)**, where n = len(text) and m = len(pat).
- Here `m ≤ 8`, so this is effectively **O(n)**.

**Going further (for long patterns):** algorithms like **KMP** and **Z-function** find a pattern in O(n + m) by never re-comparing characters they have already matched. CPython's `in` uses an optimized search that is fast in practice.

## 3. Using the guarantees in the statement

"The input will contain **at most one** of the three substrings." So the order of the `if` checks doesn't matter. Without this guarantee you would need a rule for which keyword wins.

> **Lesson:** always read the constraints and guarantees. They often remove edge cases.

## 4. Worked examples

**Easy:**
```
S = "what_is_a_graph"   → contains "what_is"
→ "what_is_a_graphdo_not_hallucinate"
```

**Tricky:**
```
S = "resolver"   → contains "solve"? r-e-[s-o-l-v-e]-r → YES
→ "resolvermake_no_mistakes"
```
The keyword can be hidden **inside** another word.

**Fallback:**
```
S = "hello_world" → no keyword → "hello_worldthank_you"
```

## 5. Common mistakes
- Adding a space or `_` between `S` and the phrase. The statement says not to.
- Reading input with trailing `\r\n` on Windows files. `.strip()` handles it.
- Using `S.find(x) > 0` instead of `>= 0`. That misses matches at index 0.

## 6. Practice
- LeetCode 28: *Find the Index of the First Occurrence in a String* (then learn KMP).
