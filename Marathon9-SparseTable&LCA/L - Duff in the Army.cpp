#include <bits/stdc++.h>

using namespace std;

const int LOG = 17;
vector<vector<int>> tree;
vector<int> depth;
vector<vector<int>> anc;
vector<vector<vector<int>>> citizens;

void merge(vector<int>& a, vector<int>& b) {
    int new_size = (int)min(a.size() + b.size(), 10ULL);
    vector<int> t(new_size);
    int i = 0, j = 0, k = 0;
    while (i < a.size() && j < b.size() && k < new_size) {
        if (a[i] < b[j]) {
            t[k++] = a[i++];
        }
        else {
            t[k++] = b[j++];
        }
    }
    while (i < a.size() && k < new_size) {
        t[k++] = a[i++];
    }
    while (j < b.size() && k < new_size) {
        t[k++] = b[j++];
    }
    a = t;
}

void merge_citizens(vector<int>& ids, int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    int k = depth[u] - depth[v];
    for (int j = LOG - 1; j >= 0; j--) {
        if (k & (1 << j)) {
            merge(ids, citizens[u][j]);
            u = anc[u][j];
        }
    }
    if (u == v) {
        merge(ids, citizens[u][0]);
        return;
    }

    for (int j = LOG - 1; j >= 0; j--) {
        if (anc[u][j] != anc[v][j]) {
            merge(ids, citizens[u][j]);
            merge(ids, citizens[v][j]);
            u = anc[u][j];
            v = anc[v][j];
        }
    }

    merge(ids, citizens[u][1]);
    merge(ids, citizens[v][0]);
}

void dfs(int u, int p) {
    anc[u][0] = p;
    for (int j = 1; j < LOG; j++) {
        anc[u][j] = anc[anc[u][j - 1]][j - 1];
        merge(citizens[u][j], citizens[u][j - 1]);
        merge(citizens[u][j], citizens[anc[u][j - 1]][j - 1]);
    }
    for (int v : tree[u]) {
        if (v != p) {
            depth[v] = depth[u] + 1;
            dfs(v, u);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, q;
    cin >> n >> m >> q;
    tree.assign(n + 1, vector<int>());
    depth.assign(n + 1, 0);
    anc.assign(n + 1, vector<int>(LOG));
    citizens.assign(n + 1, vector<vector<int>>(LOG));
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        tree[u].emplace_back(v);
        tree[v].emplace_back(u);
    }
    vector<int> id(1);
    for (int i = 1; i <= m; i++) {
        int city;
        cin >> city;
        id[0] = i;
        merge(citizens[city][0], id);
    }

    dfs(1, 1);

    vector<int> ids;
    for (int i = 0; i < q; i++) {
        int u, v, a;
        cin >> u >> v >> a;
        merge_citizens(ids, u, v);
        int k = min(a, (int)ids.size());
        cout << k << " ";
        for (int j = 0; j < k; j++) {
            cout << ids[j] << " ";
        }
        cout << "\n";
        ids.clear();
    }
    return 0;
}
