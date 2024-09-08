#include <bits/stdc++.h>

using namespace std;

vector<int> parent, sz;
unordered_map<int, int> size_cnt;

int find_set(int u) {
    if (parent[u] == u) return u;
    return parent[u] = find_set(parent[u]);
}

bool merge_set(int u, int v) {
    u = find_set(u);
    v = find_set(v);
    if (u == v) return false;
    if (sz[u] < sz[v]) swap(u, v);
    size_cnt[sz[u]]--;
    size_cnt[sz[v]]--;
    if (!size_cnt[sz[u]]) size_cnt.erase(sz[u]);
    if (!size_cnt[sz[v]]) size_cnt.erase(sz[v]);
    size_cnt[sz[u] += sz[v]]++;
    parent[v] = u;
    return true;
}

int main() {
    int n;
    cin >> n;

    parent.resize(n);
    sz.assign(n, 1);
    vector<bool> in_dsu(n);
    vector<pair<int, int>> a(n);

    for (int i = 0; i < n; i++) {
        cin >> a[i].first;
        a[i].second = i;
        parent[i] = i;
    }
    std::sort(a.begin(), a.end());

    int mx_conn_comps = 0, k = 0, edges = 0;
    for (int i = 0; i < n; i++) {
        int day_index = a[i].second;
        in_dsu[day_index] = true;
        size_cnt[1]++;
        if (day_index - 1 >= 0 && in_dsu[day_index - 1]) {
            if (merge_set(day_index - 1, day_index)) edges++;
        }
        if (day_index + 1 < n && in_dsu[day_index + 1]) {
            if (merge_set(day_index + 1, day_index)) edges++;
        }
        int conn_comps = i + 1 - edges;
        if (size_cnt.size() == 1 && conn_comps > mx_conn_comps) {
            k = a[i].first + 1;
            mx_conn_comps = conn_comps;
        }
    }
    cout << k << endl;
    return 0;
}
