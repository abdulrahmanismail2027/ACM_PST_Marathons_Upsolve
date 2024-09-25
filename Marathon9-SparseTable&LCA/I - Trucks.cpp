#include <bits/stdc++.h>

using namespace std;

int LOG = 15;

vector<vector<pair<int, int>>> tree;
vector<int> depth;
vector<vector<int>> anc;
vector<vector<int>> min_w;

struct edge {
    int u, v, w;
    edge(int a, int b, int c) {
        u = a;
        v = b;
        w = c;
    }
    bool operator<(edge& e) const {
        return w > e.w;
    }
};

class DSU {
private:
    vector<int> parent;
public:
    explicit DSU(int n) {
        parent.assign(n, -1);
    }

    int find(int u) {
        if (parent[u] < 0) return u;
        return parent[u] = find(parent[u]);
    }

    bool merge(int u, int v) {
        u = find(u);
        v = find(v);
        if (u == v) return false;
        if (parent[u] > parent[v]) swap(u, v);
        parent[u] += parent[v];
        parent[v] = u;
        return true;
    }
};

void dfs(int u, int p) {
    anc[u][0] = p;
    for (int j = 1; j < LOG; j++) {
        anc[u][j] = anc[anc[u][j - 1]][j - 1];
        min_w[u][j] = min(min_w[u][j - 1], min_w[anc[u][j - 1]][j - 1]);
    }
    for (auto [v, w] : tree[u]) {
        if (v != p) {
            min_w[v][0] = w;
            depth[v] = depth[u] + 1;
            dfs(v, u);
        }
    }
}

int find_min_w(int u, int v) {
    int mn = INT_MAX;
    if (depth[u] < depth[v]) swap(u, v);
    int k = depth[u] - depth[v];
    for (int j = LOG - 1; j >= 0; j--) {
        if (k & (1 << j)) {
            mn = min(mn, min_w[u][j]);
            u = anc[u][j];
        }
    }
    if (u == v) return mn;

    for (int j = LOG - 1; j >= 0; j--) {
        if (anc[u][j] != anc[v][j]) {
            mn = min(mn, min(min_w[u][j], min_w[v][j]));
            u = anc[u][j];
            v = anc[v][j];
        }
    }
    return min(mn, min(min_w[u][0], min_w[v][0]));
}

int main() {
    int N, M, S;
    while (cin >> N >> M >> S) {

        vector<edge> edges;
        tree.assign(N + 1, vector<pair<int, int>>());
        depth.assign(N + 1, 0);
        anc.assign(N + 1, vector<int>(LOG));
        min_w.assign(N + 1, vector<int>(LOG));

        for (int i = 0; i < M; i++) {
            int A, B, W;
            cin >> A >> B >> W;
            edges.emplace_back(A, B, W);
        }

        std::sort(edges.begin(), edges.end());
        DSU dsu(N + 1);
        for (auto [u, v, w] : edges) {
            if (dsu.merge(u, v)) {
                tree[u].emplace_back(v, w);
                tree[v].emplace_back(u, w);
            }
        }

        dfs(1, 1);

        for (int i = 0; i < S; i++) {
            int L, H;
            cin >> L >> H;
            cout << find_min_w(L, H) << "\n";
        }
    }
    return 0;
}
