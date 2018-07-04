const int LN = 17;
int lca[LN][N], depth[N];

namespace LCA {
    void dfs(int cur, int prev = -1, int _depth = 0)
    {
        lca[0][cur] = prev;
        depth[cur] = _depth;
        for(int i = 0; i < adj[cur].size(); i++) if(adj[cur][i] != prev)
                dfs(adj[cur][i], cur, _depth + 1);
    }
    int LCA(int u, int v)
    {
        if(depth[u] < depth[v]) swap(u, v);
        int diff = depth[u] - depth[v];
        for(int i = 0; i < LN; i++) if((diff >> i) & 1) u = lca[i][u];
        if(u == v) return u;
        for(int i = LN - 1; i >= 0; i--) if(lca[i][u] != lca[i][v])
        {
            u = lca[i][u];
            v = lca[i][v];
        }
        return lca[0][u];
    }
    void LCA()
    {
        memset(lca,-1,sizeof(lca));
        dfs(1);
        for(int i=1;i<LN;i++) for(int j=1;j<=n;j++)
            if(lca[i-1][j]!=-1) lca[i][j]=lca[i-1][lca[i-1][j]];
    }
}
