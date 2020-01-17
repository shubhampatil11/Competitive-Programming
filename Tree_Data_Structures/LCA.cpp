struct lca {
    vector<vector<int>> &graph;
    vector<vector<int>> par;
    vector<int> depth;
    int n, m;
    int root;

    void dfs(int cur, int prev = 0, int ht = 0) {
        par[cur][0] = prev;
        depth[cur] = ht;
        for(int u : graph[cur]) {
            if(u != prev) {
                dfs(u, cur, ht + 1);
            }
        }
        return;
    }

    lca(vector<vector<int>> &_graph, int _root = 1) : graph(_graph), root(_root) {
        n = (int)graph.size();
        m = (int)log2(n) + 2;
        par.resize(n);
        for(int i = 0; i < n; i++) {
            par[i].resize(m);
        }
        depth.resize(n, 0);
        dfs(root);

        for(int i = 1; i < m; i++) {
            for(int j = 0; j < n; j++) {
                par[j][i] = par[par[j][i - 1]][i - 1];
            }
        }
    }

    int get(int u, int v) {
        if(depth[u] > depth[v]) {
            swap(u, v);
        }
        int diff = depth[v] - depth[u];
        for(int i = m - 1; i >= 0; i--) {
            if((diff >> i) & 1) {
                v = par[v][i];
            }
        }
        if(u == v) {
            return u;
        }
        for(int i = m - 1; i >= 0; i--) {
            if(par[u][i] != par[v][i]) {
                u = par[u][i];
                v = par[v][i];
            }
        }
        return par[u][0];
    }
};
