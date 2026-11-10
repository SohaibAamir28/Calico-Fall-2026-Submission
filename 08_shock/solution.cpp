// CALICO Fall '26 - Problem 8: shock (That Was an Earthquake? I Barely Felt It)
//
// Idea: process the earthquakes in REVERSE order.
//   A seismometer's final reading comes from the LAST earthquake that reached
//   it. Going backwards, the first earthquake that touches a cell is exactly
//   that last one, so every cell is written at most ONCE and then never again.
//
//   Earthquake i touches a disk: row r gets columns C_i +- isqrt(P_i^2 - dr^2).
//   To skip already-written cells inside such a row segment quickly, every row
//   keeps a "next unwritten column" union-find (nxt). After writing column c
//   we link c -> c+1, so later searches jump over it.
#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>
#include <string>
using namespace std;

// ---------- fast input: read the whole stdin into a buffer once ----------
static char ibuf[1 << 25];
static size_t ilen = 0, ipos = 0;
static inline int readInt() {
    while (ipos < ilen && (ibuf[ipos] < '0' || ibuf[ipos] > '9') && ibuf[ipos] != '-') ipos++;
    bool neg = false;
    if (ibuf[ipos] == '-') { neg = true; ipos++; }
    int x = 0;
    while (ipos < ilen && ibuf[ipos] >= '0' && ibuf[ipos] <= '9') x = x * 10 + (ibuf[ipos++] - '0');
    return neg ? -x : x;
}

// nxt[r*W + c] = parent pointer of column c in row r's union-find.
// The root of c is the first unwritten column >= c (column M = sentinel).
static vector<int> nxt;
static int W; // row stride = M + 1 (one extra sentinel column per row)

// Returns the first unwritten column >= c in the row starting at `base`,
// with path compression (iterative).
static inline int findNext(int base, int c) {
    int r = c;
    while (nxt[base + r] != r) r = nxt[base + r];
    while (nxt[base + c] != r) { int t = nxt[base + c]; nxt[base + c] = r; c = t; }
    return r;
}

// Exact integer square root: largest w with w*w <= x.
// sqrt() on doubles can be off by one, so we correct it.
static inline long long isqrtll(long long x) {
    long long w = (long long)sqrt((double)x);
    while (w > 0 && w * w > x) w--;
    while ((w + 1) * (w + 1) <= x) w++;
    return w;
}

int main() {
    ilen = fread(ibuf, 1, sizeof(ibuf), stdin);
    int T = readInt();
    string out;
    out.reserve(1 << 24);
    char tmp[16];
    while (T--) {
        int N = readInt(), M = readInt(), Q = readInt();
        vector<int> R(Q), C(Q), P(Q);
        for (int i = 0; i < Q; i++) { R[i] = readInt(); C[i] = readInt(); P[i] = readInt(); }

        // Every column starts unwritten: each one is its own root.
        W = M + 1;
        nxt.assign((size_t)N * W, 0);
        for (int r = 0; r < N; r++)
            for (int c = 0; c <= M; c++) nxt[(size_t)r * W + c] = c;
        vector<long long> val((size_t)N * M, 0);   // final readings (0 = never shaken)

        for (int i = Q - 1; i >= 0; i--) {          // reverse order!
            long long p2 = (long long)P[i] * P[i];
            // Shock level is positive  <=>  dr^2 + dc^2 <= P^2.
            for (int dr = -P[i]; dr <= P[i]; dr++) {
                int r = R[i] + dr;
                if (r < 0 || r >= N) continue;
                long long rem = p2 - (long long)dr * dr;   // allowed dc^2
                long long w = isqrtll(rem);                // half width of this row
                int lo = (int)max(0LL, C[i] - w), hi = (int)min((long long)M - 1, C[i] + w);
                int base = r * W;
                // Visit ONLY the unwritten cells in [lo, hi].
                int c = findNext(base, lo);
                while (c <= hi) {
                    long long dc = c - C[i];
                    val[(size_t)r * M + c] = rem - dc * dc + 1;   // = P^2 - dist^2 + 1
                    nxt[base + c] = c + 1;                       // mark as written
                    c = findNext(base, c + 1);
                }
            }
        }

        // Print the grid (flush the buffer now and then to limit memory).
        for (int r = 0; r < N; r++) {
            for (int c = 0; c < M; c++) {
                if (c) out.push_back(' ');
                int len = snprintf(tmp, sizeof(tmp), "%lld", val[(size_t)r * M + c]);
                out.append(tmp, len);
            }
            out.push_back('\n');
        }
        if (out.size() > (1u << 23)) { fwrite(out.data(), 1, out.size(), stdout); out.clear(); }
    }
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
