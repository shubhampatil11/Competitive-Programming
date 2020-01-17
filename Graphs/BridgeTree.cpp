struct BridgeTree {
    struct edge {
        int u, v;
        edge(int _u, int _v) : u(_u), v(_v) {}
        int other(int x) {
            return x == u ? v : u;
        }
    };
    int n, m;
    vector<vector<int>> graph;
    vector<edge> E;
    vector<bool> visited, is_bridge;
    vector<int> tin, low;
    int timer, component;
    vector<vector<int>> newgraph;
    vector<int> group;

    BridgeTree(int _n = N) : n(_n) {
        graph.resize(n + 1);
        group.resize(n + 1);
        newgraph.resize(n + 1);
        component = 0;
        m = 0;
    }

    void add_edge(int u, int v) {
        E.push_back(edge(u, v));
        graph[u].push_back(m);
        graph[v].push_back(m);
        m++;
    }

    void IS_BRIDGE(int x) {
        is_bridge[x] = true;
    }

    void dfs(int cur, int par = -1) {
        visited[cur] = true;
        tin[cur] = low[cur] = timer++;
        for (int to : graph[cur]) {
            int x = to;
            to = E[to].other(cur);
            if (to == par) {
                continue;
            }
            if (visited[to]) {
                low[cur] = min(low[cur], tin[to]);
            }
            else {
                dfs(to, cur);
                low[cur] = min(low[cur], low[to]);
                if (low[to] > tin[cur])
                    IS_BRIDGE(x);
            }
        }
    }

    void find_bridges() {
        timer = 0;
        visited.assign(n + 1, false);
        is_bridge.assign(m, false);
        tin.assign(n + 1, -1);
        low.assign(n + 1, -1);
        for(int i = 1; i <= n; i++) {
            if(!visited[i]) {
                dfs(i);
            }
        }
    }

    void run(int cur) {
        queue<int> q;
        visited[cur] = true;
        q.push(cur);
        group[cur] = component;
        while(!q.empty()) {
            int u = q.front();
            q.pop();
            for(int x : graph[u]) {
                int v = E[x].other(u);
                if(visited[v]) {
                    continue;
                }
                if(is_bridge[x]) {
                    component++;
                    newgraph[component].push_back(group[cur]);
                    newgraph[group[cur]].push_back(component);
                    run(v);
                }
                else {
                    group[v] = group[cur];
                    q.push(v);
                    visited[v] = true;
                }
            }
        }
    }

    void solve() {
        find_bridges();
        visited.assign(n + 1, false);
        for(int i = 1; i <= n; i++) {
            if(!visited[i]) {
                component++;
                run(i);
            }
        }
    }
};
