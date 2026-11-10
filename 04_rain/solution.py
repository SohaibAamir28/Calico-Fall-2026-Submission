# CALICO Fall '26 - Problem 4: rain (Drone Rain)
#
# Key observations:
#  - Two drones moving in the SAME direction keep the same relative offset,
#    and no two drones start on the same (X, Y), so they never stack.
#  - An East drone (Xe+t, Ye) and a North drone (Xn, Yn+t) are stacked at
#    night t  iff  t = Xn - Xe = Ye - Yn,  i.e.  Xe+Ye == Xn+Yn  and  t = Xn-Xe.
#    So every E/N pair meets at most once, and at most 2 drones share a point.
#  - Each meeting on a night t in [0, K-1] costs exactly 1 unit (the lower
#    drone is covered that night).
#
# Answer = N*K - #(E/N pairs with the same X+Y and 0 <= Xn - Xe <= K-1)
import sys
from bisect import bisect_left, bisect_right
from collections import defaultdict


def solve(N, K, L):
    # Group drones by the diagonal X + Y; only drones on the same diagonal
    # can ever meet. Inside a group we only need the X coordinate.
    east = defaultdict(list)     # X+Y -> list of X of East drones
    north = defaultdict(list)    # X+Y -> list of X of North drones
    for X, Y, Z, D in L:
        if D == "E":
            east[X + Y].append(X)
        else:
            north[X + Y].append(X)

    lost = 0                     # number of (drone, night) pairs without rain
    for s, xs_n in north.items():
        xs_e = east.get(s)
        if not xs_e:
            continue
        xs_e.sort()
        for xn in xs_n:
            # They meet on night t = xn - xe, which must lie in [0, K-1]:
            #   xn - (K-1) <= xe <= xn   -> count with two binary searches.
            lost += bisect_right(xs_e, xn) - bisect_left(xs_e, xn - (K - 1))
    # Every drone collects 1 unit per night, except when it is covered.
    return N * K - lost


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    T = int(data[idx]); idx += 1
    out = []
    for _ in range(T):
        N = int(data[idx]); K = int(data[idx + 1]); idx += 2
        L = []
        for _ in range(N):
            # X, Y, Z (height) and direction D ("E" or "N")
            L.append((int(data[idx]), int(data[idx + 1]), int(data[idx + 2]), data[idx + 3].decode()))
            idx += 4
        out.append(str(solve(N, K, L)))
    print("\n".join(out))


if __name__ == "__main__":
    main()
