# CALICO Fall '26 - Problem 5: galaxy (Super Ben Galaxy) - main + bonus
#
# State = (cell, mode), mode 0 = blue, 1 = red.
# Every move and every spin can be undone, so "can I reach X?" is the same as
# "are these two states connected?" -> union-find (DSU) over all states.
# A normal tile's two states are merged, because you can spin there.
import sys


def solve(N, M, Q, G, start, end):
    # State id of (r, c, mode) = (r*M + c)*2 + mode
    S = N * M * 2
    parent = list(range(S))

    def find(x):
        # Find the root, then compress the path (iterative, no recursion).
        root = x
        while parent[root] != root:
            root = parent[root]
        while parent[x] != root:
            parent[x], x = root, parent[x]
        return root

    def union(a, b):
        ra, rb = find(a), find(b)
        if ra != rb:
            parent[ra] = rb

    def active(ch, mode):
        # Can we stand on tile `ch` while in `mode`?
        return ch == "." or (ch == "B" and mode == 0) or (ch == "R" and mode == 1)

    # Build connectivity. Looking only right and down covers every edge once.
    for r in range(N):
        row = G[r]
        for c in range(M):
            ch = row[c]
            base = (r * M + c) * 2
            if ch == ".":
                union(base, base + 1)            # spin: blue <-> red
            for mode in (0, 1):
                if not active(ch, mode):
                    continue
                if c + 1 < M and active(row[c + 1], mode):
                    union(base + mode, base + 2 + mode)          # right neighbour
                if r + 1 < N and active(G[r + 1][c], mode):
                    union(base + mode, base + 2 * M + mode)      # down neighbour

    res = []
    for (sr, sc), (er, ec) in zip(start, end):
        s = find((sr * M + sc) * 2)   # start tile is normal; Ben starts in blue mode
        eb = (er * M + ec) * 2        # destination's blue-mode state id
        ch = G[er][ec]
        # Reaching the destination in EITHER mode (where it is active) counts.
        ok = False
        if active(ch, 0) and find(eb) == s:
            ok = True
        if not ok and active(ch, 1) and find(eb + 1) == s:
            ok = True
        res.append("YES" if ok else "NO")
    return res


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    T = int(data[idx]); idx += 1
    out = []
    for _ in range(T):
        N, M, Q = int(data[idx]), int(data[idx + 1]), int(data[idx + 2]); idx += 3
        G = [data[idx + i].decode() for i in range(N)]; idx += N     # grid rows
        start, end = [], []
        for _ in range(Q):
            start.append((int(data[idx]), int(data[idx + 1])))
            end.append((int(data[idx + 2]), int(data[idx + 3])))
            idx += 4
        out.extend(solve(N, M, Q, G, start, end))
    print("\n".join(out))


if __name__ == "__main__":
    main()
