namespace CutPoints {

set<int> cutpoints;
vector<bool> visited;
vector<int> tin, low;
int timer = 0;

void IS_CUTPOINT(int u) {
    cutpoints.insert(u);
}

void dfs(vector<vector<int>> &adj, int v, int p = -1) {
    visited[v] = true;
    tin[v] = low[v] = timer++;
    int children=0;
    for (int to : adj[v]) {
        if (to == p) {
            continue;
        }
        if (visited[to]) {
            low[v] = min(low[v], tin[to]);
        }
        else {
            dfs(adj, to, v);
            low[v] = min(low[v], low[to]);
            if (low[to] >= tin[v] && p != -1) {
                IS_CUTPOINT(v);
            }
            ++children;
        }
    }
    if(p == -1 && children > 1) {
        IS_CUTPOINT(v);
    }
    return;
}

void find_cutpoints(vector<vector<int>> &adj) {
    cutpoints.clear();
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
