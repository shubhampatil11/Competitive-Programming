namespace Bridge {

vector<pair<int,int>> bridges;
vector<bool> visited;
vector<int> tin, low;
int timer = 0;

void IS_BRIDGE(int u, int v) {
    bridges.push_back({u, v});
}

void dfs(vector<vector<int>> &adj, int cur, int par = -1) {
    visited[cur] = true;
    tin[cur] = low[cur] = timer++;
    for (int to : adj[cur]) {
        if (to == par) {
            continue;
        }
        if (visited[to]) {
            low[cur] = min(low[cur], tin[to]);
        } else {
            dfs(adj, to, cur);
            low[cur] = min(low[cur], low[to]);
            if (low[to] > tin[cur])
                IS_BRIDGE(cur, to);
        }
    }
}

void find_bridges(vector<vector<int>> &adj) {
    bridges.clear();
    timer = 0;
    int n = (int)adj.size() - 1;
    visited.assign(n + 1, false);
    tin.assign(n + 1, -1);
    low.assign(n + 1, -1);
    for(int i = 1; i <= n; i++) {
        if(!visited[i]) {
            dfs(adj, i);
        }
    }
}

}
