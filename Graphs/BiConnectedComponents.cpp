struct BCC {
    vector<vector<int>> graph;
    vector<bool> visited;
    vector<int> tin, low;
    int n, timer;
    stack<pair<int,int>> stk;

    vector<bool> is_cutpoints;
    vector<int> cutpoints;
    vector<vector<pair<int,int>>> edges;

    BCC(int _n = N) : n(_n) {
        graph.resize(n + 1);
        is_cutpoints.resize(n + 1, false);
    }
    BCC(vector<vector<int>> &_graph) {
        n = (int)_graph.size();
        graph = _graph;
        is_cutpoints.resize(n + 1, false);
    }

    void add_edge(int u, int v) {
        assert(u >= 0 && u <= n && v >= 0 && v <= n);
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    void create(int to = -1, int v = -1) {
        pair<int,int> it = make_pair(to, v);
        vector<pair<int,int>> temp;
        while(!stk.empty()) {
            temp.push_back(stk.top());
            if(stk.top() == it) {
                break;
            }
            stk.pop();
        }
        if(to != -1) {
            stk.pop();
        }
        edges.push_back(temp);
    }

    void IS_CUTPOINT(int u) {
        if(!is_cutpoints[u]) {
            is_cutpoints[u] = true;
            cutpoints.push_back(u);
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
                stk.push({to, v});
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
                stk.push({to, v});
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
                if(!stk.empty()) {
                    create();
                }
            }
        }
    }
};
