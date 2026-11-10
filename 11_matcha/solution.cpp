// CALICO Fall '26 - Problem 11: matcha (A Matcha a Day)
//
// Maximize f_1 ^ ... ^ f_N with L_i <= f_i <= R_i.
//
// Greedy from the top bit b down to 0. We keep a list of ranges (all values
// < 2^(b+1)) and an XOR offset c that the low bits of the answer must absorb.
//  * Range entirely below 2^b: its bit b is 0.
//  * Range entirely at/above 2^b: its bit b is 1 (strip the bit, flip parity).
//  * "Spanning" range (L < 2^b <= R): it may choose either half.
//      upper half -> low bits in [0, R-2^b]                (a prefix range)
//      lower half -> low bits in [L, mask] = mask ^ [0, mask-L]
// If no range spans, bit b of the answer is forced. Otherwise bit b = 1 is
// always achievable (and beats anything with 0), which fixes the parity of
// how many spanning ranges take the upper half, so the parity of the `mask`
// terms added to c is fixed too.
//
// The XOR of several prefix ranges [0, x_i] is again a prefix range [0, z],
// computed greedily (zMerge), and a bigger z is always better (superset).
// So all spanning ranges collapse into ONE range [0, Z], where Z is the best
// z over side choices with the required parity:
//   - every element takes its larger option; if the parity is wrong, exactly
//     one element k takes its smaller option instead (max over k);
//   - z depends only on the ~31 largest values, so with more than 40
//     elements the parity fix costs nothing.
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
using namespace std;

// ---------- fast input ----------
static char ibuf[1 << 25];
static size_t ilen = 0, ipos = 0;
static inline long long readInt() {
    while (ipos < ilen && (ibuf[ipos] < '0' || ibuf[ipos] > '9')) ipos++;
    long long x = 0;
    while (ipos < ilen && ibuf[ipos] >= '0' && ibuf[ipos] <= '9') x = x * 10 + (ibuf[ipos++] - '0');
    return x;
}

// Index of the highest set bit (x > 0).
static inline int topBit(long long x) { return 63 - __builtin_clzll((unsigned long long)x); }

// Max XOR of y_i with 0 <= y_i <= v_i  (and the XOR set is exactly [0, result]).
// Take the largest value x with top bit h:
//   - if another value also reaches bit h, the two can make every number
//     below 2^(h+1), so all lower bits become 1;
//   - otherwise bit h is set and x continues as x - 2^h.
static long long zMerge(vector<long long> v) {
    if (v.size() > 40) {   // only the largest ~31 values can matter
        nth_element(v.begin(), v.begin() + 40, v.end(), greater<long long>());
        v.resize(40);
    }
    priority_queue<long long> pq(v.begin(), v.end());
    long long res = 0;
    while (!pq.empty()) {
        long long x = pq.top(); pq.pop();
        if (x == 0) break;
        int h = topBit(x);
        if (!pq.empty() && pq.top() >= (1LL << h)) { res |= (2LL << h) - 1; break; }
        res |= 1LL << h;
        pq.push(x - (1LL << h));
    }
    return res;
}

int main() {
    ilen = fread(ibuf, 1, sizeof(ibuf), stdin);
    int T = (int)readInt();
    string out;
    char tmp[24];
    vector<pair<long long, long long>> cur, nxt;   // ranges for this bit / next bit
    vector<long long> optA, optB;                  // spanning ranges: upper / lower option
    while (T--) {
        int N = (int)readInt();
        cur.assign(N, {0, 0});
        for (int i = 0; i < N; i++) cur[i].first = readInt();
        for (int i = 0; i < N; i++) cur[i].second = readInt();
        long long c = 0, ans = 0;
        for (int b = 29; b >= 0; b--) {
            long long half = 1LL << b, mask = half - 1;
            nxt.clear(); optA.clear(); optB.clear();
            int parity = 0;                        // XOR of the forced bit-b values
            for (auto& pr : cur) {
                long long L = pr.first, R = pr.second;
                if (R < half) nxt.push_back({L, R});                                 // bit b = 0
                else if (L >= half) { nxt.push_back({L - half, R - half}); parity ^= 1; } // bit b = 1
                else { optA.push_back(R - half); optB.push_back(mask - L); }         // spanning
            }
            int cb = (int)((c >> b) & 1);          // bit b of the offset
            if (optA.empty()) {
                // Nothing to choose: bit b of the answer is forced.
                if (cb ^ parity) ans |= half;
                c &= mask;
            } else {
                ans |= half;                       // bit b = 1 is achievable
                int s = (int)optA.size();
                int pUp = 1 ^ cb ^ parity;          // required parity of #upper choices
                int lowPar = (s ^ pUp) & 1;          // parity of #lower choices
                c = (c & mask) ^ (lowPar ? mask : 0);  // each lower choice adds `mask`
                long long Z;
                if (s == 1) {
                    // A single spanning range: its side is forced by the parity.
                    Z = pUp ? optA[0] : optB[0];
                } else {
                    // Mx = better option, Mn = worse option of each spanning range.
                    vector<long long> Mx(s), Mn(s);
                    int defUp = 0; bool tie = false;   // parity of "upper" when all take Mx
                    for (int i = 0; i < s; i++) {
                        Mx[i] = max(optA[i], optB[i]);
                        Mn[i] = min(optA[i], optB[i]);
                        if (optA[i] > optB[i]) defUp ^= 1;
                        else if (optA[i] == optB[i]) tie = true;  // a tie can fix parity for free
                    }
                    if (tie || defUp == pUp || s > 40) {
                        Z = zMerge(Mx);
                    } else {
                        // Parity is wrong: exactly one element switches to its worse option.
                        Z = -1;
                        for (int k = 0; k < s; k++) {
                            long long keep = Mx[k];
                            Mx[k] = Mn[k];
                            Z = max(Z, zMerge(Mx));
                            Mx[k] = keep;
                        }
                    }
                }
                nxt.push_back({0, Z});             // all spanning ranges -> one range [0, Z]
            }
            swap(cur, nxt);
        }
        int len = snprintf(tmp, sizeof(tmp), "%lld\n", ans);
        out.append(tmp, len);
    }
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
