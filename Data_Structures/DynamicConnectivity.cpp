struct dsu_save {
    int v, rnkv, u, rnku;
    dsu_save() {}
    dsu_save(int _v, int _rnkv, int _u, int _rnku)
        : v(_v), rnkv(_rnkv), u(_u), rnku(_rnku) {}
};

struct dsu_with_rollbacks {
    vector<int> p, rnk;
    int comps;
    stack<dsu_save> op;

    dsu_with_rollbacks() {}

    dsu_with_rollbacks(int n) {
        p.resize(n);
        rnk.resize(n);
        for (int i = 0; i < n; i++) {
            p[i] = i;
            rnk[i] = 1;
        }
        comps = n;
    }

    int find_set(int v) {
        return (v == p[v]) ? v : find_set(p[v]);
    }

    bool unite(int v, int u) {
        v = find_set(v);
        u = find_set(u);
        if (v == u) {
            return false;
        }
        comps--;
        if (rnk[v] > rnk[u]) {
            swap(v, u);
        }
        op.push(dsu_save(v, rnk[v], u, rnk[u]));
        p[v] = u;
        if (rnk[u] == rnk[v]) {
            rnk[u] += rnk[v];
        }
        return true;
    }

    void rollback() {
        if (op.empty()) {
            return;
        }
        dsu_save x = op.top();
        op.pop();
        comps++;
        p[x.v] = x.v;
        rnk[x.v] = x.rnkv;
        p[x.u] = x.u;
        rnk[x.u] = x.rnku;
    }
};

struct query {
    int v, u;
    bool united;
    query(int _v, int _u) : v(_v), u(_u) {}
};

struct QueryTree {
    vector<vector<query>> t;
    dsu_with_rollbacks dsu;
    int T;

    QueryTree() {}

    QueryTree(int _T, int n) : T(_T) {
        dsu = dsu_with_rollbacks(n);
        t.resize(4 * T + 4);
    }

    void add_to_tree(int node, int start, int end, int l, int r, query& q) {
        if (end < l || start > r) {
            return;
        }
        if (start >= l && end <= r) {
            t[node].push_back(q);
            return;
        }
        int mid = (start + end) >> 1;
        add_to_tree(2 * node, start, mid, l, r, q);
        add_to_tree(2 * node + 1, mid + 1, end, l, r, q);
    }

    void add_query(query q, int l, int r) {
        add_to_tree(1, 0, T - 1, l, r, q);
    }

    void dfs(int node, int l, int r, vector<int>& ans) {
        for (query &q : t[node]) {
            q.united = dsu.unite(q.v, q.u);
        }
        if (l == r) {
            ans[l] = dsu.comps;
        }
        else {
            int mid = (l + r) >> 1;
            dfs(2 * node, l, mid, ans);
            dfs(2 * node + 1, mid + 1, r, ans);
        }
        for (query q : t[node]) {
            if (q.united)
                dsu.rollback();
        }
    }
};
