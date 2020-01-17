namespace hld {
const int LOG = log2(N) + 2;
int depth[N],subtree[N];
int baseArray[N];
int chainHead[N], chainInd[N], posInBase[N], otherend[N];
int cnt, chainNo;
int par[N][LOG];
vector<int> adj[N];
vector<int> cost[N];
vector<int> edge[N];
segtree T;

/**v is an ancestor of u**/
int query_up(int u, int v) {
    if(u == v) {
        return 0;
    }
    int uchain, vchain = chainInd[v];
    int ans = -1;
    while(1) {
        uchain = chainInd[u];
        if(uchain == vchain) {
            if(u == v) {
                break;
            }
            ans = max(ans, T.query(1, 0, cnt - 1, posInBase[v] + 1, posInBase[u]).sum);
            break;
        }

        ans = max(ans, T.query(1, 0, cnt - 1, posInBase[chainHead[uchain]], posInBase[u]).sum);

        u = chainHead[uchain];
        u = par[u][0];
    }
    return ans;
}

void HLD(int cur, int prev, int _cost) {
    if(chainHead[chainNo] == -1) {
        chainHead[chainNo] = cur;
    }
    chainInd[cur] = chainNo;
    baseArray[cnt] = _cost;
    posInBase[cur] = cnt;
    cnt++;
    int sc = -1, ncost;
    for(int i = 0; i < (int)adj[cur].size(); i++) {
        if(adj[cur][i] != prev) {
            if(sc == -1 || subtree[sc] <= subtree[adj[cur][i]]) {
                sc = adj[cur][i];
                ncost = cost[cur][i];
            }
        }
    }

    if(sc != -1) {
        HLD(sc, cur, ncost);
    }

    for(int i = 0; i < (int)adj[cur].size(); i++) {
        if(adj[cur][i] != prev && adj[cur][i] != sc) {
            chainNo++;
            HLD(adj[cur][i], cur, cost[cur][i]);
        }
    }
}

void dfs(int cur, int prev, int _depth = 0) {
    par[cur][0] = prev;
    depth[cur] = _depth;
    subtree[cur] = 1;
    for(int i = 0; i < adj[cur].size(); i++) {
        if(adj[cur][i] != prev) {
            otherend[edge[cur][i]] = adj[cur][i];
            dfs(adj[cur][i], cur, _depth + 1);
            subtree[cur] += subtree[adj[cur][i]];
        }
    }
}

void solve(int n) {
    dfs(1, 0);
    HLD(1, -1, -1);
    for(int i = 1; i < LOG; i++) {
        for(int j = 0; j <= n; j++) {
            par[j][i] = par[par[j][i - 1]][i - 1];
        }
    }
    return;
}

int lca(int u, int v) {
    if(depth[u] > depth[v]) {
        swap(u, v);
    }
    int diff = depth[v] - depth[u];
    for(int i = LOG - 1; i >= 0; i--) {
        if((diff >> i) & 1) {
            v = par[v][i];
        }
    }
    if(u == v) {
        return u;
    }
    for(int i = LOG - 1; i >= 0; i--) {
        if(par[u][i] != par[v][i]) {
            u = par[u][i];
            v = par[v][i];
        }
    }
    return par[u][0];
}

void reset(int n) {
    cnt = 0; chainNo = 0;
    for(int i = 0; i <= n; i++) {
        adj[i].clear();
        cost[i].clear();
        edge[i].clear();
        chainHead[i] = -1;
        for(int j = 0; j < LOG; j++) {
            par[i][j] = 0;
        }
    }
    return;
}

void build() {
    T = segtree(cnt);
    for(int i = 0; i < cnt; i++) {
        T.A[i] = baseArray[i];
    }
    T.build(1, 0, cnt - 1);
}

}
using namespace hld;
