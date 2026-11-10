# Learn: Problem 2 — smoke (Valorant)

**Concepts:** simulation, event-driven thinking, state machines, invariants · **Difficulty:** ⭐

---

## 1. Simulation: two ways to do it

### (a) Tick by tick (naive)
Advance time one second at a time and update everything.

- Cost: **O(max timestamp)**. With timestamps up to 10⁹ this is far too slow.

### (b) Event-driven (what the solution does)
Nothing interesting happens *between* requests. The state only needs updating **when a request arrives**, so we jump straight from one request to the next.

- Cost: **O(N)**.

> **Analogy:** a security guard doesn't need to stare at the door every second. A doorbell tells them when someone arrives, and they react only then.

## 2. Modeling the state

All we need to remember:

| Variable | Meaning |
|---|---|
| `charges` | How many smokes are ready now (0, 1 or 2) |
| `ready_at` | When the charge that is **currently recharging** will be ready |

This is a tiny **state machine**:

```
        use (start timer)         use (timer keeps running)
  [2] ─────────────────────▶ [1] ─────────────────────▶ [0]
   ▲                           │ ▲                         │
   └──── timer done ───────────┘ └──── timer done ─────────┘
                                   (and a new timer starts at once)
```

> **Analogy:** one phone charger and two batteries. Only one battery charges at a time. When it's full, you plug in the second one straight away.

## 3. Processing order matters

"Recharges are processed **before** requests at the same timestamp."

So inside the loop, the **recharge step comes first**, then the request. Swapping these two steps gives wrong answers when a charge finishes exactly at a request time.

## 4. Worked examples (R = 10)

**Example A: requests at 0, 1, 10, 20**

| t | Recharge step | Charges before use | Action | `ready_at` after |
|---|---|---|---|---|
| 0 | — | 2 | full → start timer, use | 10 |
| 1 | 10 > 1, nothing | 1 | use | 10 |
| 10 | 10 ≤ 10 → +1 (now 1), next timer → 20 | 1 | use | 20 |
| 20 | 20 ≤ 20 → +1 (now 1), next timer → 30 | 1 | use | 30 |

Answer: **YES**

**Example B: requests at 0, 1, 10, 11**

At t = 11, the timer says 20, so no charge has come back and `charges = 0`. Answer: **NO**.

**Complex: a long gap refills both charges**

Requests at 0, 1, 100 with R = 10:
- After t = 1, `charges = 0` and `ready_at = 10`.
- At t = 100, the `while` loop runs **twice**: at 10 → 1 charge (next timer 20), and at 20 → 2 charges (stop, we're full).
- Since we are full, using a charge starts a new timer at 110.

This is why it's a `while` loop and not an `if`.

## 5. Invariant thinking

An **invariant** is a fact that stays true at every step. Here:

> *"If `charges < 2`, then `ready_at` is the exact time the next charge comes back."*

Each line of the code keeps this true. Checking invariants like this is how you convince yourself (and an interviewer) that a simulation is correct.

## 6. How this solution was verified
A second-by-second brute force (the naive simulation) was run on thousands of random small inputs and compared with this solution. All results matched.

## 7. Practice
- LeetCode 495: *Teemo Attacking* (a similar timeline simulation).
- LeetCode 621: *Task Scheduler* (cooldown reasoning).
