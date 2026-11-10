// CALICO Fall '26 - Problem 9: galaxy2 (Super Ben Galaxy 2)
//
// Shortest path on states (cell, mode): a move costs 1 s, a spin costs 1 s.
// There are at most N <= 10 rows, so we use DIVIDE & CONQUER ON COLUMNS:
//   - A node covers columns [l, r] and has a middle column `mid`.
//   - Run a BFS (restricted to columns [l, r]) from every active state in
//     column mid (at most 2N sources).
//   - Every query whose two endpoints both lie in [l, r] updates
//         best = min over sources of  d(src, start) + d(src, end).
//   - Queries entirely left of mid go to the left child, entirely right of
//     mid go to the right child.
// Why it is correct: take the deepest node whose column range contains the
// whole optimal path. That path must cross the node's middle column, and the
// restricted BFS distances there are exact. The query is evaluated at every
// node on its way down, so it is evaluated at that node too.
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// ---------- fast input ----------
static char ibuf[1 << 25];
static size_t ilen = 0, ipos = 0;
static inline void skipWs() { while (ipos < ilen && (ibuf[ipos] == ' ' || ibuf[ipos] == '\n' || ibuf[ipos] == '\r' || ibuf[ipos] == '\t')) ipos++; }
static inline long long readInt() {
    skipWs();
    long long x = 0;
    while (ipos < ilen && ibuf[ipos] >= '0' && ibuf[ipos] <= '9') x = x * 10 + (ibuf[ipos++] - '0');
    return x;
}
static inline string readTok() {
    skipWs();
    size_t st = ipos;
    while (ipos < ilen && ibuf[ipos] != ' ' && ibuf[ipos] != '\n' && ibuf[ipos] != '\r' && ibuf[ipos] != '\t') ipos++;
    return string(ibuf + st, ibuf + ipos);
}

static const int INF = 1e9;
static int N, M;
static vector<string> G;                          // the grid
static vector<int> qsr, qsc, qer, qec, best;      // query start/end and best distance so far
static vector<int> distBuf, bfsQ;                 // reused buffers (BFS distances / queue)

static inline bool active(char ch, int mode) { // mode 0 = blue, 1 = red
    return ch == '.' || (ch == 'B' && mode == 0) || (ch == 'R' && mode == 1);
}

// Handle the queries `qs` whose endpoints are both inside columns [l, r].
static void rec(int l, int r, vector<int>& qs) {
    if (qs.empty() || l > r) return;
    int mid = (l + r) / 2;
    int width = r - l + 1;
    int segSize = width * N * 2;                  // number of states inside [l, r]
    // Local state id inside this segment.
    auto sid = [&](int row, int col, int mode) { return ((col - l) * N + row) * 2 + mode; };

    // BFS sources: every active state in the middle column.
    vector<int> sources;
    for (int row = 0; row < N; row++)
        for (int mode = 0; mode < 2; mode++)
            if (active(G[row][mid], mode)) sources.push_back(sid(row, mid, mode));

    // One BFS per source; distances stored one after another in distBuf.
    int S = (int)sources.size();
    distBuf.assign((size_t)S * segSize, INF);
    if ((int)bfsQ.size() < segSize) bfsQ.resize(segSize);
    static const int dr[4] = {1, -1, 0, 0}, dc[4] = {0, 0, 1, -1};
    for (int si = 0; si < S; si++) {
        int* d = distBuf.data() + (size_t)si * segSize;
        int head = 0, tail = 0;
        d[sources[si]] = 0;
        bfsQ[tail++] = sources[si];
        while (head < tail) {
            int u = bfsQ[head++];
            int mode = u & 1, cell = u >> 1;
            int row = cell % N, col = cell / N + l;
            int du = d[u] + 1;
            // Spin (only on normal tiles): flip the mode bit.
            if (G[row][col] == '.') {
                int v = u ^ 1;
                if (d[v] > du) { d[v] = du; bfsQ[tail++] = v; }
            }
            // Walk to a neighbour that is active in the current mode and in [l, r].
            for (int k = 0; k < 4; k++) {
                int nr = row + dr[k], nc = col + dc[k];
                if (nr < 0 || nr >= N || nc < l || nc > r) continue;
                if (!active(G[nr][nc], mode)) continue;
                int v = sid(nr, nc, mode);
                if (d[v] > du) { d[v] = du; bfsQ[tail++] = v; }
            }
        }
    }

    // Update every query through the middle column, then split the queries.
    vector<int> left, right;
    for (int q : qs) {
        int s = sid(qsr[q], qsc[q], 0);       // start: normal tile, blue mode
        int e0 = sid(qer[q], qec[q], 0), e1 = e0 + 1;
        char ech = G[qer[q]][qec[q]];
        bool a0 = active(ech, 0), a1 = active(ech, 1);   // destination usable in which modes
        int b = best[q];
        for (int si = 0; si < S; si++) {
            const int* d = distBuf.data() + (size_t)si * segSize;
            int ds = d[s];                    // graph is undirected: d(src,start) = d(start,src)
            if (ds >= INF) continue;
            int de = INF;
            if (a0) de = min(de, d[e0]);
            if (a1) de = min(de, d[e1]);
            if (de >= INF) continue;
            b = min(b, ds + de);
        }
        best[q] = b;
        if (qsc[q] < mid && qec[q] < mid) left.push_back(q);
        else if (qsc[q] > mid && qec[q] > mid) right.push_back(q);
        // Otherwise the query crosses mid: its answer is already final.
    }
    vector<int>().swap(qs);                   // free memory before recursing
    rec(l, mid - 1, left);
    rec(mid + 1, r, right);
}

int main() {
    ilen = fread(ibuf, 1, sizeof(ibuf), stdin);
    int T = (int)readInt();
    string out;
    while (T--) {
        N = (int)readInt(); M = (int)readInt(); int Q = (int)readInt();
        G.assign(N, string());
        for (int i = 0; i < N; i++) G[i] = readTok();
        qsr.assign(Q, 0); qsc.assign(Q, 0); qer.assign(Q, 0); qec.assign(Q, 0);
        vector<long long> K(Q);
        best.assign(Q, INF);
        for (int i = 0; i < Q; i++) {
            qsr[i] = (int)readInt(); qsc[i] = (int)readInt();
            qer[i] = (int)readInt(); qec[i] = (int)readInt();
            K[i] = readInt();                 // time limit for this query
        }
        vector<int> qs(Q);
        for (int i = 0; i < Q; i++) qs[i] = i;
        rec(0, M - 1, qs);
        for (int i = 0; i < Q; i++) out += (best[i] < INF && best[i] <= K[i]) ? "YES\n" : "NO\n";
    }
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
