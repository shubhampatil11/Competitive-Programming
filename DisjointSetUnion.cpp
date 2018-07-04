class wunionfind
{
public:
    int *id, *sz;
    wunionfind(int n = N)
    {
        id = new int[n + 1];
        sz = new int[n + 1];
        int i;
        for(i = 0; i <= n; i++) id[i] = i, sz[i] = 1;
    }
    int root(int idx)
    {
        int i = idx;
        while(i != id[i]) id[i] = id[id[i]], i = id[i];
        return i;
    }
    void uni(int a, int b)
    {
        int i = root(a), j = root(b);
        if (i != j)
        {
            if(sz[i] >= sz[j]) id[j] = i, sz[i] += sz[j], sz[j] = 0;
            else id[i] = j, sz[j] += sz[i], sz[i] = 0;
        }
    }
    bool check(int a, int b)
    {
        if(root(a) == root(b)) return true;
        return false;
    }
};
