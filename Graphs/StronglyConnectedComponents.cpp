struct SCC {
    int n, nScc;
    vector<int> visited, idx, order;
    vector<vector<int>> graph, rgraph;

    SCC(int _n = N) {
        n = _n;
        graph.resize(n + 1);
        rgraph.resize(n + 1);
        visited.resize(n + 1);
        idx.resize(n + 1);
        order.resize(n + 1);
        for (int i = 0; i <= n; i++) {
            graph[i].clear();
            rgraph[i].clear();
        }
    }

    void add_edge(int u, int v) {
        graph[u].push_back(v);
        rgraph[v].push_back(u);
    }

    void DFS(int u) {
        visited[u] = 1;
        for (auto v : graph[u]) {
            if (!visited[v]) {
                DFS(v);
            }
        }
        order.push_back(u);
    }

    void rDFS(int u) {
        visited[u] = 1;
        idx[u] = nScc;
        for (auto v : rgraph[u]) {
            if (!visited[v]) {
                rDFS(v);
            }
        }
    }

    void solve() {
        nScc = 0;
        order.clear();
        for (int i = 1; i <= n; i++) {
            visited[i] = 0;
        }
        for (int i = 1; i <= n; i++) {
            if (!visited[i]) {
                DFS(i);
            }
        }
        reverse(order.begin(), order.end());
        for (int i = 1; i <= n; i++) {
            visited[i] = 0;
        }
        for (auto v : order) {
            if (!visited[v]) {
                rDFS(v);
                nScc++;
            }
        }
    }
};

