# CALICO Fall '26 - Problem 2: smoke (Valorant)
#
# Omen has at most 2 smoke charges. A used charge takes R seconds to come
# back, and charges recharge ONE AT A TIME (the second one only starts after
# the first is restored). We only need to look at the moments when a request
# arrives, so we simulate request by request.
import sys


def solve(N, R, A):
    charges = 2          # charges currently available (starts full)
    ready_at = 0         # time when the charge that is recharging now comes back
    for t in A:
        # 1) Apply every recharge that finished at or before time t.
        #    (Recharges happen BEFORE a request at the same timestamp.)
        while charges < 2 and ready_at <= t:
            charges += 1
            if charges < 2:
                # One more charge is still missing: its recharge starts the
                # moment the previous one finished.
                ready_at += R
        # 2) No charge left -> this request cannot be served.
        if charges == 0:
            return "NO"
        # 3) If we were full, nothing was recharging; using a charge now
        #    starts a new recharge timer.
        if charges == 2:
            ready_at = t + R
        # 4) Use the smoke.
        charges -= 1
    return "YES"


def main():
    # Fast input: read every token at once.
    data = sys.stdin.buffer.read().split()
    idx = 0
    T = int(data[idx]); idx += 1
    out = []
    for _ in range(T):
        N = int(data[idx]); R = int(data[idx + 1]); idx += 2
        A = [int(x) for x in data[idx:idx + N]]; idx += N   # request timestamps
        out.append(solve(N, R, A))
    print("\n".join(out))


if __name__ == "__main__":
    main()
