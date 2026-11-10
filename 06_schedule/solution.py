# CALICO Fall '26 - Problem 6: schedule (Average Berkeley CS Major)
#
# Prerequisites form a forest (each course has <= 1 parent). Let depth(root)=1.
# (1) A prerequisite-closed set S can be finished in M semesters with at most
#     K courses per semester  iff  for every h >= 1:
#         #{v in S : depth(v) >= h} <= K * (M - h + 1)
#     (Hu's level bound for trees, applied in reversed time: the courses of
#      depth >= h need the last M-h+1 semesters at most.)
# (2) Exchange argument: swapping a deepest leaf of S for a shallower available
#     course never breaks (1). So some optimal S = ALL courses of depth < D
#     plus SOME courses of depth D. We try every D and keep the best.
import sys


def solve(N, M, K, P):
    # ---- depth of every course (iterative, avoids recursion limits) ----
    depth = [0] * (N + 1)
    for v in range(1, N + 1):
        if depth[v]:
            continue
        # Walk up until we hit a root (P = 0) or an already-computed node.
        chain = []
        u = v
        while u != 0 and depth[u] == 0:
            chain.append(u)
            u = P[u - 1]
        d = depth[u] if u != 0 else 0
        # Assign depths back down the chain.
        for w in reversed(chain):
            d += 1
            depth[w] = d

    # cnt[d] = number of courses with depth exactly d
    maxd = max(depth[1:])
    cnt = [0] * (maxd + 2)
    for v in range(1, N + 1):
        cnt[depth[v]] += 1

    # For a fixed D (take all of depth < D, x of depth D):
    #   slack_h(D) = K*(M-h+1) - sum_{h<=d<D} cnt[d]      for h = 1..D
    #   x(D)       = min(cnt[D], min_h slack_h(D))
    # min_slack is updated incrementally when D grows, so the loop is O(maxd).
    best = 0
    taken = 0                # courses with depth < D (all of them are taken)
    min_slack = None
    for D in range(1, min(maxd, M) + 1):   # depth D needs at least D semesters
        new_slack = K * (M - D + 1)        # slack of the new level h = D
        if min_slack is None:
            min_slack = new_slack
        else:
            # Older levels lose cnt[D-1] slack (that layer is now fully taken).
            min_slack = min(min_slack - cnt[D - 1], new_slack)
        if min_slack < 0:
            break                          # taking all of depth < D is infeasible
        x = min(cnt[D], min_slack)         # how many depth-D courses fit
        best = max(best, taken + x)
        if x < cnt[D]:
            break            # layer D can't be completed -> deeper D impossible
        taken += cnt[D]
    return best


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    T = int(data[idx]); idx += 1
    out = []
    for _ in range(T):
        N, M, K = int(data[idx]), int(data[idx + 1]), int(data[idx + 2]); idx += 3
        P = [int(x) for x in data[idx:idx + N]]; idx += N   # P[i-1] = prereq of i
        out.append(str(solve(N, M, K, P)))
    print("\n".join(out))


if __name__ == "__main__":
    main()
