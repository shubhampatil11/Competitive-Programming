struct ST{
    int sum;
    ST(int sum = 0) : sum(sum) {}
};

ST operator + (const ST &A, const ST &B) {
	/**Merging Logic**/
    return (ST(...));
}

struct segtree {
    vector<ST> tree;
    vector<int> A;
    segtree(int n = N) {
        tree.resize(4 * n);
        A.resize(n);
    }

    void build(int node, int start, int end) {
        if(start == end) {
            tree[node] = ST(A[start]);
        }
        else {
            int mid = (start + end) >> 1;
            build(2 * node, start, mid);
            build(2 * node + 1, mid+1, end);
            tree[node] = tree[2 * node] + tree[2 * node + 1];
        }
    }

    void update(int node, int start, int end, int idx, ll val) {
        if(start == end) {
            tree[node].sum += val;
        }
        else {
            int mid = (start + end) >> 1;
            if(idx <= mid) update(2 * node, start, mid, idx, val);
            else update(2 * node + 1, mid + 1, end, idx, val);
            tree[node] = tree[2 * node] + tree[2 * node + 1];
        }
    }

    ST query(int node, int start, int end, int l, int r) {
        if(start >= l && end <= r) return tree[node];
        int mid = (start + end) >> 1;
        if(mid >= r) return query(2 * node, start, mid, l, r);
        if(mid < l) return query(2 * node + 1, mid + 1, end, l, r);
        ST P1 = query(2 * node, start, mid, l, r);
        ST P2 = query(2 * node + 1, mid + 1, end, l, r);
        return P1 + P2;
    }
};
