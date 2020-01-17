struct BlockCutTree {
    vector<vector<int>> graph;
    vector<bool> visited;
    vector<int> tin, low;
    int n, timer;
    stack<pair<int,int>> edges;

    vector<vector<int>> newgraph;
    vector<bool> is_cutpoints;
    vector<int> cutpoints;
    int cnt;
    vector<set<int>> block;
    vector<int> M, CompId;

    BlockCutTree(int _n = N) : n(_n) {
        graph.resize(n + 1);
        cnt = 0;
        M.resize(n + 1, -1);
        CompId.resize(n + 1, -1);
        is_cutpoints.resize(n + 1, false);
        block.resize(n + 1);
    }
    BlockCutTree(vector<vector<int>> &_graph) {
        n = (int)_graph.size();
        graph = _graph;
        is_cutpoints.resize(n + 1, false);
        cnt = 0;
        M.resize(n + 1, -1);
        CompId.resize(n + 1, -1);
        block.resize(n + 1);
    }

    void add_edge(int u, int v) {
        assert(u >= 0 && u <= n && v >= 0 && v <= n);
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    void create(int to = -1, int v = -1) {
        int B = cnt; cnt++;
        newgraph.push_back(vector<int>());
        pair<int,int> it = make_pair(to, v);
        while(!edges.empty() && edges.top() != it) {
            block[B].insert(edges.top().second);
            block[B].insert(edges.top().first);
            edges.pop();
        }
        if(to != -1) {
            edges.pop();
            block[B].insert(to);
            block[B].insert(v);
        }
        for(int u : block[B]) {
            if(is_cutpoints[u]) {
                newgraph[B].push_back(M[u]);
                newgraph[M[u]].push_back(B);
            }
            else {
                CompId[u] = B;
            }
        }
    }

    void IS_CUTPOINT(int u) {
        if(!is_cutpoints[u]) {
            is_cutpoints[u] = true;
            cutpoints.push_back(u);
            M[u] = cnt; cnt++;
            block[M[u]].insert(u);
            newgraph.push_back(vector<int>());
            CompId[u] = M[u];
        }
    }

    void dfs(int v, int p = -1) {
        visited[v] = true;
        tin[v] = low[v] = timer++;
        int children = 0;
        for (int to : graph[v]) {
            if (to == p) {
                continue;
            }
            if(!visited[to]) {
                edges.push({to, v});
                dfs(to, v);
                low[v] = min(low[v], low[to]);
                children++;
                if ((low[to] >= tin[v] && p != -1) || (p == -1 && children > 1)) {
                    IS_CUTPOINT(v);
                    create(to, v);
                }
            }
            else if(low[to] < tin[v]) {
                low[v] = min(low[v], low[to]);
                edges.push({to, v});
            }
        }
        return;
    }

    void solve() {
        timer = 0;
        visited.assign(n + 1, false);
        tin.assign(n + 1, -1);
        low.assign(n + 1, -1);
        for(int i = 1; i <= n; i++) {
            if(!visited[i]) {
                dfs(i);
                if(!edges.empty()) {
                    create();
                }
            }
        }
    }
};
