# CALICO Fall '26 - Problem 7: forgor (Big Ben Forgor) - RUN-TWICE problem
#
# Phase 1 (FIRST):  given the tree, output N integers.
#     Root the tree at 1.  A_i = parent(i) for i >= 2,
#                          A_1 = XOR of all parents.
#     (parents < 2^18 <= 10^6, so the XOR also stays within the value limit)
# Phase 2 (SECOND): the same list comes back with ONE value replaced by -1.
#     If A_1 was erased, every parent is still known.
#     Otherwise the erased parent = A_1 XOR (all other known parents),
#     because x ^ x = 0 cancels every known parent out of the checksum.
import sys
from collections import deque


def solve_phase1(N, E):
    # Adjacency list of the tree.
    adj = [[] for _ in range(N + 1)]
    for u, v in E:
        adj[u].append(v)
        adj[v].append(u)
    # BFS from vertex 1 to find every vertex's parent.
    par = [0] * (N + 1)
    seen = [False] * (N + 1)
    seen[1] = True
    dq = deque([1])
    while dq:
        u = dq.popleft()
        for w in adj[u]:
            if not seen[w]:
                seen[w] = True
                par[w] = u
                dq.append(w)
    # Checksum = XOR of every parent.
    x = 0
    for i in range(2, N + 1):
        x ^= par[i]
    # Output: [checksum, parent(2), parent(3), ..., parent(N)]
    return [x] + par[2:]


def solve_phase2(N, A):
    if A[0] != -1:
        # A parent was erased: rebuild it from the checksum.
        missing = A.index(-1)
        x = A[0]
        for i in range(1, N):
            if i != missing:
                x ^= A[i]
        A = A[:]
        A[missing] = x
    # Edge (i, parent(i)) for every vertex i >= 2  (A[i] is vertex i+1's parent).
    return [(i + 1, A[i]) for i in range(1, N)]


def main():
    data = sys.stdin.buffer.read().split()
    phase = data[0].decode()        # "FIRST" or "SECOND"
    N = int(data[1])
    if phase == "FIRST":
        E = [(int(data[2 + 2 * i]), int(data[3 + 2 * i])) for i in range(N - 1)]
        print(" ".join(map(str, solve_phase1(N, E))))
    else:
        A = [int(x) for x in data[2:2 + N]]
        print("\n".join(f"{u} {v}" for u, v in solve_phase2(N, A)))


if __name__ == "__main__":
    main()
