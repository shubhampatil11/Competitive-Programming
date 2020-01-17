namespace CD {
int decomposed[N], subtree[N], sz = 0;
bool visited[N] = {false};

void dfs(int cur, int prev) {
    subtree[cur] = 1;
    sz++;
    for(auto u : adj[cur]) {
        if(u != prev && !visited[u]) {
            dfs(u, cur);
            subtree[cur] += subtree[u];
        }
    }
}

int centroid(int cur, int prev) {
    for(auto u : adj[cur]) {
        if(u != prev && !visited[u] && subtree[u] > sz / 2) {
            return centroid(u, cur);
        }
    }
    return cur;
}

void decomposition(int root, int prev) {
    sz = 0;
    dfs(root, -1);
    int C = centroid(root, -1);
    if(prev == -1) {
        decomposed[C] = C;
    }
    else {
        decomposed[C] = prev;
    }
    visited[C] = true;
    for(auto u : adj[C]) {
        if(!visited[u]) {
            decomposition(u, C);
        }
    }
}
}

/*
    call in main():
    CD::decomposition(1,-1);
*/
