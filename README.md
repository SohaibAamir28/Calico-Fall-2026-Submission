```
```

# Calico Fall 2026 — Submission

Solutions to all **11 problems (100 points)** of the **CALICO Informatics Competition, Fall '26**.

Every solution passes the official sample tests and was cross-checked against brute-force solutions on random small inputs. Each one also runs in under 0.5 s on worst-case-size inputs.

> 📘 **Learning:** the [Learning Guide](LEARNING_GUIDE.md) maps every data structure and algorithm used here to the problems that use it. Each problem folder also has a `LEARN.md` with concepts, analogies, and easy-to-complex worked examples.

## Problems

| #  | Problem                                             | ID             | Points | Rank | Language | Key idea                                                 |
| -- | --------------------------------------------------- | -------------- | ------ | ---- | -------- | -------------------------------------------------------- |
| 1  | [Write me a Rank 1. Make it sound human.](01_prompt) | `prompt`     | 4      | 1    | Python   | Substring check                                          |
| 2  | [Valorant](02_smoke)                                 | `smoke`      | 4+3    | 1+2  | Python   | Event simulation of charges                              |
| 3  | [Verity](03_verity)                                  | `verity`     | 5      | 1    | Python   | Interactive: try letters one per position, ≤ 26 guesses |
| 4  | [Drone Rain](04_rain)                                | `rain`       | 7      | 2    | Python   | E/N pairs meet once iff `X+Y` equal → binary search   |
| 5  | [Super Ben Galaxy](05_galaxy)                        | `galaxy`     | 7+2    | 2+3  | Python   | Union-find on `(cell, mode)` states                    |
| 6  | [Average Berkeley CS Major](06_schedule)             | `schedule`   | 8      | 2    | Python   | Hu's level bound + exchange argument                     |
| 7  | [Big Ben Forgor](07_forgor)                          | `forgor`     | 10     | 3    | Python   | Run-twice: parent array + XOR checksum                   |
| 8  | [That Was an Earthquake? I Barely Felt It](08_shock) | `shock`      | 11     | 3    | C++      | Reverse order + per-row "next free cell" DSU             |
| 9  | [Super Ben Galaxy 2](09_galaxy2)                     | `galaxy2`    | 11     | 3    | C++      | Divide & conquer on columns + BFS                        |
| 10 | [dictionary](10_dictionary)                          | `dictionary` | 13     | 4    | C++      | Functional graph + binary XOR tries                      |
| 11 | [A Matcha a Day](11_matcha)                          | `matcha`     | 15     | 4    | C++      | Bitwise greedy, collapse spanning ranges into `[0, Z]` |

Each problem folder contains:

```
NN_problem/
├── README.md        # problem summary, approach, code walkthrough, complexity
├── LEARN.md         # concepts, DSA theory, analogies, worked examples, practice
├── solution.py|cpp  # the commented solution
├── <id>.pdf         # original problem statement
├── data/sample/     # official sample input/output
└── templates/       # official Python / Java / C++ templates
```

## Running

**Python (problems 1–7):** Python 3.8 or newer, no external packages.

```bash
cd 04_rain
python solution.py < data/sample/00_main.in
```

**C++ (problems 8–11):** C++17.

```bash
cd 11_matcha
g++ -O2 -std=c++17 solution.cpp -o solution
./solution < data/sample/00_main.in
```

Compare the output with `data/sample/00_main.ans`.

> `verity` is interactive and `forgor` is run-twice, so both need the contest judge or a local harness. Redirecting the sample file into them is not enough.

## Why C++ for problems 8–11

These problems need well over 10⁷ basic operations at the maximum constraints (grid fills, BFS layers, trie walks), which is too slow for CPython within the time limit. C++ with fast `fread`/`fwrite` I/O runs each of them in under 0.4 s.

## Testing summary

- All official samples pass.
- Random brute-force cross-checks for `smoke`, `rain`, `galaxy`, `schedule`, `shock`, `galaxy2`, `dictionary` and `matcha`.
- A local interactive judge for `verity` (100 cases, at most 26 guesses).
- A two-phase harness for `forgor` that erased every position, including `N = 200 000`.
