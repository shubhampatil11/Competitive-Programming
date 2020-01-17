struct node {
    int l, r, lazy, sum;
    node(int _l = 0, int _r = 0, int _lazy = 0, int _sum = 0) :
        l(_l), r(_r), lazy(_lazy), sum(_sum) {}
};

struct STLazy {
    vector<node> T;
    vector<int> A;

    STLazy(int n = N) {
        T.resize(4 * n);
        A.resize(n);
    }

    void pushdown(int node) {
    	/** Lazy Logic **/
        if(T[node].lazy) {
            T[node].sum += (T[node].r - T[node].l + 1) * T[node].lazy;
            if(T[node].l < T[node].r) {
                T[2 * node].lazy += T[node].lazy;
                T[2 * node + 1].lazy += T[node].lazy;
            }
            T[node].lazy = 0;
        }
    }

    void pushup(int node) {
    	/** Merging Logic **/
        T[node].sum = T[2 * node].sum + T[2 * node + 1].sum;
    }

    void build(int node, int start, int end) {
        T[node].l = start; T[node].r = end; T[node].lazy = 0;
        if(start == end) {
            T[node].sum = A[start];
            return;
        }
        int mid = (start + end) >> 1;
        build(2 * node, start, mid);
        build(2 * node + 1, mid + 1, end);
        pushup(node);
    }

    void update(int node, int start, int end, int l, int r, int val) {
        pushdown(node);
        if(start > r || end < l) {
            return;
        }
        if(start >= l && end <= r) {
            T[node].lazy += val;
            pushdown(node);
            return;
        }
        int mid = (start + end) >> 1;
        update(2 * node, start, mid, l, r, val);
        update(2 * node + 1, mid + 1, end, l, r, val);
        pushup(node);
    }

    int query(int node, int start, int end, int l, int r) {
        if(start > r || end < l) {
            return 0;
        }
        pushdown(node);
        if(start >= l && end <= r) {
            return T[node].sum;
        }
        int mid = (start + end) >> 1;
        return query(2 * node, start, mid, l, r) + query(2 * node + 1, mid + 1, end, l, r);
    }
};
