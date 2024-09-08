#include <bits/stdc++.h>

using namespace std;

const int LOG = 18;

struct edge {
    int u, v, w, i;
    edge(int a, int b, int c, int n) {
        u = a; v = b; w = c; i = n;
    }
    bool operator<(const edge& e) const {
        return w < e.w;
    }
};

class DS {
public:
    vector<int> parent;

    explicit DS(int n) {
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
        if (parent[u] > parent[v]) {
            swap(u, v);
        }
        parent[u] += parent[v];
        parent[v] = u;
        return true;
    }
};

void dfs(int u, vector<int>& depth, vector<pair<int, int>>& parent,
         const vector<vector<pair<int, int>>>& tree, vector<bool>& visited) {
    visited[u] = true;
    for (auto [v, w] : tree[u]) {
        if (!visited[v]) {
            depth[v] = depth[u] + 1;
            parent[v] = {u, w};
            dfs(v, depth, parent, tree, visited);
        }
    }
}

int find_lca(int u, int v, const vector<int>& depth,
             const vector<vector<int>>& anc, const vector<vector<int>>& max_weight) {
    int max_w = 0;
    if (depth[u] < depth[v]) swap(u, v);
    int k = depth[u] - depth[v];
    for (int j = LOG; j >= 0; j--) {
        if (k & (1 << j)) {
            max_w = max(max_w, max_weight[u][j]);
            u = anc[u][j];
        }
    }

    if (u != v) {

        for (int j = LOG; j >= 0; j--) {
            if (anc[u][j] != anc[v][j]) {
                max_w = max(max_w, max_weight[u][j]);
                max_w = max(max_w, max_weight[v][j]);
                u = anc[u][j];
                v = anc[v][j];
            }
        }
        max_w = max(max_w, max_weight[u][0]);
        max_w = max(max_w, max_weight[v][0]);
    }
    return max_w;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;

    vector<edge> edges;
    vector<edge> hold;
    vector<vector<int>> anc(n + 1, vector<int>(LOG + 1));
    vector<vector<int>> max_weight(n + 1, vector<int>(LOG + 1));
    vector<vector<pair<int, int>>> tree(n + 1);
    vector<bool> visited(n + 1);
    vector<int> depth(n + 1);
    vector<pair<int, int>> parent(n + 1);
    DS ds(n + 1);

    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        edges.emplace_back(u, v ,w, i);
    }

    sort(edges.begin(), edges.end());

    long long W = 0;
    for (edge e : edges) {
        auto [u, v, w, i] = e;
        if (ds.merge(u, v)) {
            tree[u].emplace_back(v, w);
            tree[v].emplace_back(u, w);
            W += w;
        }
        else {
            hold.push_back(e);
        }
    }

    parent[1] = {1, 0};
    dfs(1, depth, parent, tree, visited);

    for (int j = 0; j <= LOG; j++) {
        for (int i = 1; i <= n; i++) {
            if (j == 0) {
                anc[i][0] = parent[i].first;
                max_weight[i][0] = parent[i].second;
            }
            else {
                anc[i][j] = anc[anc[i][j - 1]][j - 1];
                max_weight[i][j] = max(max_weight[i][j - 1], max_weight[anc[i][j - 1]][j - 1]);
            }
        }
    }

    vector<long long> result(m, W);

    for (auto [u, v, w, i] : hold) {
        int max_w = find_lca(u, v, depth, anc, max_weight);
        result[i] = result[i] - max_w + w;
    }

    for (int i = 0; i < m; i++) cout << result[i] << "\n";

    return 0;
}