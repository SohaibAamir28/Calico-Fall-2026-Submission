# Learn: Problem 4 — rain (Drone Rain)

**Concepts:** invariants in motion, counting the complement, hashing / grouping, sorting, binary search · **Difficulty:** ⭐⭐

---

## 1. Count what you *lose*, not what you *get*

Without any stacking, every drone collects 1 unit every night, for **N·K** in total. So:

```
answer = N·K − (number of (drone, night) pairs where the drone is covered)
```

Counting the "bad" events is often much easier than counting the "good" ones. This is called **complementary counting**.

> **Analogy:** to count the students present, count the absent ones and subtract from the class size.

## 2. When can two drones meet? (invariants)

| Pair | Their difference over time | Can they ever meet? |
|---|---|---|
| East & East | Both get X+1 each night, so the offset is **constant** | Never (they start apart) |
| North & North | Both get Y+1, so the offset is constant | Never |
| East & North | Explained below | **At most once** |

East drone at night t: `(Xe + t, Ye)`. North drone: `(Xn, Yn + t)`.

They meet when `Xe + t = Xn` **and** `Ye = Yn + t`. Then:
- `t = Xn − Xe` and `t = Ye − Yn`
- Adding gives **`Xe + Ye = Xn + Yn`**.

So **only drones on the same anti-diagonal (`X + Y` equal) can ever meet**, and on night `t = Xn − Xe`.

**Invariant view:** each night, both kinds of drone increase `X + Y` by exactly 1. So the *difference* in `X + Y` between any two drones never changes. If it starts nonzero, they can never be at the same point.

> **Analogy:** two cars, one driving east and one driving north, toward the same crossroads. They collide only if both reach the crossing at the same moment, which depends on their distances to it.

## 3. From pairs to fast counting

A pair (E, N) on the same diagonal costs 1 unit if `0 ≤ Xn − Xe ≤ K − 1`, i.e.

```
Xn − (K − 1) ≤ Xe ≤ Xn
```

### Step 1: group with a hash map
`east[X+Y] → list of Xe` and `north[X+Y] → list of Xn`. Building the groups is O(N) with a `defaultdict(list)`.

### Step 2: count in a range with binary search
For each North drone, count the East `Xe` values (in the same group) inside `[Xn − K + 1, Xn]`. After **sorting**, that is two binary searches:

```python
count = bisect_right(xs, hi) - bisect_left(xs, lo)
```

- `bisect_left(xs, lo)` = the first index whose value is `≥ lo`
- `bisect_right(xs, hi)` = the first index whose value is `> hi`
- The difference is the number of elements in `[lo, hi]`.

> **Analogy:** finding all names between "Kh" and "Kz" in a phone book. You don't read every page; you jump to where "Kh" starts and where "Kz" ends.

## 4. Worked examples

**Easy:** K = 5, East at (0, 2), North at (2, 0)
- X+Y: 2 and 2, the same diagonal ✔
- They meet at t = Xn − Xe = 2. Check: East at (2, 2), North at (2, 2) ✔
- 2 ≤ K−1 = 4, so it counts. Answer = 2·5 − 1 = **9**.
- With K = 2 (nights 0 and 1), they never meet in time. Answer = **4**.

**Complex:** one diagonal with East `Xe = [0, 1, 3, 7]`, one North drone `Xn = 5`, K = 4
- Range: `[5 − 3, 5] = [2, 5]`
- `bisect_left([0,1,3,7], 2) = 2`, `bisect_right([0,1,3,7], 5) = 3` → **1** meeting (Xe = 3, at night 2).
- Xe = 7 would need t = −2 (in the past). Xe = 0 or 1 would need t = 5 or 4, beyond night K−1 = 3.

## 5. Complexity
- Brute force over all pairs: O(N²). With N = 2·10⁵ that is about 4·10¹⁰ operations, which is far too slow.
- Group + sort + binary search: **O(N log N)**.

## 6. Common mistakes
- Simulating night by night: O(N·K), and K can be huge.
- Off-by-one on the nights. They are `0 … K−1` (K nights, and the first one is before any move).
- Forgetting that `t` must be `≥ 0`, i.e. `Xe ≤ Xn`.

## 7. Practice
- LeetCode 35: *Search Insert Position* (binary search basics).
- LeetCode 2187 / 2226 (binary search on the answer, the next step after this).
- LeetCode 49: *Group Anagrams* (grouping with a hash map by a key).
