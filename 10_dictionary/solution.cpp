// CALICO Fall '26 - Problem 10: dictionary
//
// Functional graph i -> A_i. From i the walk is a TAIL leading into a CYCLE,
// then goes around the cycle forever. The happiness values we can stop at are
// the XORs of the non-empty prefixes of that walk.
//
//   W(i)  = XOR of V along the tail from i up to (not including) its cycle node.
//   Tail prefixes:   W(i) ^ W(v) for every strict ancestor v (towards the cycle),
//                    including the cycle node itself, whose W is 0.
//   Cycle prefixes:  cycle nodes c_0..c_{L-1}, Z_j = V_c0 ^ ... ^ V_cj,
//                    C = XOR of the whole cycle. Entering the cycle at c_s, the
//                    values are  W(i) ^ Z_{s-1} ^ y  with  y in {Z_e, Z_e ^ C}
//                    (going around once more adds C; twice cancels out).
//
// Both kinds are "max XOR with a set" queries -> binary tries:
//   - one static trie per cycle with every Z_j and Z_j ^ C,
//   - one insert/erase trie holding W of the current DFS ancestors.
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// ---------- fast input ----------
static char ibuf[1 << 25];
static size_t ilen = 0, ipos = 0;
static inline int readInt() {
    while (ipos < ilen && (ibuf[ipos] < '0' || ibuf[ipos] > '9')) ipos++;
    int x = 0;
    while (ipos < ilen && ibuf[ipos] >= '0' && ibuf[ipos] <= '9') x = x * 10 + (ibuf[ipos++] - '0');
    return x;
}

static const int B = 30;   // values are < 2^30

// Binary trie over 30-bit numbers with counts, so values can also be removed.
struct Trie {
    vector<int> c0, c1, cnt;   // child for bit 0, child for bit 1, #values below node
    void reset() { c0.assign(1, 0); c1.assign(1, 0); cnt.assign(1, 0); }
    int newNode() { c0.push_back(0); c1.push_back(0); cnt.push_back(0); return (int)c0.size() - 1; }
    // delta = +1 inserts x, delta = -1 removes x.
    void add(int x, int delta) {
        int u = 0;
        cnt[u] += delta;
        for (int b = B - 1; b >= 0; b--) {
            int bit = (x >> b) & 1;
            int nxt = bit ? c1[u] : c0[u];
            if (!nxt) { nxt = newNode(); if (bit) c1[u] = nxt; else c0[u] = nxt; }
            u = nxt;
            cnt[u] += delta;
        }
    }
    // max over y in trie of (x ^ y). Greedy: at each bit prefer the opposite bit.
    int maxXor(int x) const {  // trie must be non-empty
        int u = 0, res = 0;
        for (int b = B - 1; b >= 0; b--) {
            int bit = (x >> b) & 1;
            int want = bit ? c0[u] : c1[u];
            if (want && cnt[want] > 0) { res |= 1 << b; u = want; }
            else u = bit ? c1[u] : c0[u];
        }
        return res;
    }
};

int main() {
    ilen = fread(ibuf, 1, sizeof(ibuf), stdin);
    int T = readInt();
    string out;
    Trie cyc, anc;
    char tmp[16];
    while (T--) {
        int N = readInt();
        vector<int> A(N + 1), V(N + 1);
        for (int i = 1; i <= N; i++) A[i] = readInt();
        for (int i = 1; i <= N; i++) V[i] = readInt();

        // ---- 1) find the cycle nodes ----
        // state: 0 = unvisited, 1 = on the current walk, 2 = finished.
        // Meeting a node with state 1 means we just closed a NEW cycle.
        vector<int> state(N + 1, 0), onCycle(N + 1, 0);
        for (int i = 1; i <= N; i++) {
            if (state[i]) continue;
            int u = i;
            vector<int> path;
            while (!state[u]) { state[u] = 1; path.push_back(u); u = A[u]; }
            if (state[u] == 1) { // new cycle found, starting at u
                int w = u;
                do { onCycle[w] = 1; w = A[w]; } while (w != u);
            }
            for (int p : path) state[p] = 2;
        }

        // ---- 2) reverse edges of the tree parts (child lists as linked lists) ----
        vector<int> head(N + 1, 0), nxtE(N + 1, 0);
        for (int i = 1; i <= N; i++) if (!onCycle[i]) { nxtE[i] = head[A[i]]; head[A[i]] = i; }

        // ---- 3) every cycle, and the trees hanging off it ----
        vector<long long> ans(N + 1, 0);
        vector<int> Wv(N + 1, 0), done(N + 1, 0);
        vector<int> stk;
        for (int i = 1; i <= N; i++) {
            if (!onCycle[i] || done[i]) continue;
            // Collect the cycle in walking order: c_0 = i, c_1 = A[i], ...
            vector<int> cyc_nodes;
            int w = i;
            do { cyc_nodes.push_back(w); done[w] = 1; w = A[w]; } while (w != i);
            int L = (int)cyc_nodes.size();
            // Prefix XORs around the cycle.
            vector<int> Z(L);
            int acc = 0;
            for (int j = 0; j < L; j++) { acc ^= V[cyc_nodes[j]]; Z[j] = acc; }
            int Ctot = acc;
            // Cycle trie: every value a walk starting at c_0 can stop at.
            cyc.reset();
            for (int j = 0; j < L; j++) { cyc.add(Z[j], 1); cyc.add(Z[j] ^ Ctot, 1); }
            anc.reset();
            for (int s = 0; s < L; s++) {
                int r = cyc_nodes[s];
                // Starting at c_s = starting at c_0 with Z_{s-1} XORed out.
                int pre = s ? Z[s - 1] : 0;
                ans[r] = cyc.maxXor(pre);
                // Iterative DFS over the in-tree hanging off r.
                // anc holds W of every ancestor (W(r) = 0 for the cycle node).
                Wv[r] = 0;
                anc.add(0, 1);
                stk.clear();
                for (int ch = head[r]; ch; ch = nxtE[ch]) stk.push_back(ch);
                while (!stk.empty()) {
                    int u = stk.back(); stk.pop_back();
                    if (u < 0) { anc.add(Wv[-u], -1); continue; }   // leaving u: remove it
                    Wv[u] = V[u] ^ Wv[A[u]];
                    long long best = anc.maxXor(Wv[u]);              // stop inside the tail
                    long long viaCyc = cyc.maxXor(Wv[u] ^ pre);      // stop on the cycle
                    ans[u] = max(best, viaCyc);
                    anc.add(Wv[u], 1);                               // entering u
                    stk.push_back(-u);                               // marker to remove u later
                    for (int ch = head[u]; ch; ch = nxtE[ch]) stk.push_back(ch);
                }
                anc.add(0, -1);
            }
        }
        for (int i = 1; i <= N; i++) {
            int len = snprintf(tmp, sizeof(tmp), "%lld", ans[i]);
            out.append(tmp, len);
            out.push_back(i == N ? '\n' : ' ');
        }
    }
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
