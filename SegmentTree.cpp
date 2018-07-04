struct ST{
    /* Parameters of the Segment Tree */
}*tree;

ST operator + (const ST &A, const ST &B)
{
    /* Merge operation of two nodes of the Segment Tree */
}

void build(int node, int start, int end)
{
    if(start == end) { /* Initialization of the node */ ;}
    else
    {
        int mid = (start + end) >> 1;
        build(2 * node, start, mid);
        build(2 * node + 1, mid+1, end);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }
}

void update(int node, int start, int end, int idx)
{
    if(start == end) { /* Assigning updated node to the previous node */ ;}
    else
    {
        int mid = (start + end) >> 1;
        if(idx <= mid) update(2 * node, start, mid, idx);
        else update(2 * node + 1, mid + 1, end, idx);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }
}

ST query(int node, int start, int end, int l, int r)
{
    if(start >= l && end <= r) return tree[node];
    int mid = (start + end) >> 1;
    if(mid >= r) return query(2 * node, start, mid, l, r);
    if(mid < l) return query(2 * node + 1, mid + 1, end, l, r);
    ST P1 = query(2 * node, start, mid, l, r);
    ST P2 = query(2 * node + 1, mid + 1, end, l, r);
    return P1 + P2;
}
