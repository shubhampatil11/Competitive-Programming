struct node {
    int sum;
    node *left, *right;

    node(int sum = 0, node *left = NULL, node *right = NULL) :
        sum(sum), left(left), right(right) {}
};

node* build(int start, int end) {
    if(start == end) {
        return new node();
    }
    int mid = (start + end) >> 1;
    node *L = build(start, mid);
    node *R = build(mid + 1, end);
    return new node(L->sum + R->sum, L, R);
}

node* add(node *cur, int start, int end, int idx, int val) {
    if(start <= idx && idx <= end) {
        if(start == end) {
            return new node(val);
        }
        int mid = (start + end) >> 1;
        node *L = add(cur->left, start, mid, idx, val);
        node *R = add(cur->right, mid + 1, end, idx, val);
        return new node(L->sum + R->sum, L, R);
    }
    return cur;
}

int query(node *root, int start, int end, int l, int r) {
    if(end < l || start > r) {
        return 0;
    }
    if(start >= l && end <= r) {
        return root->sum;
    }
    int mid = (start + end) >> 1;
    return query(root->left, start, mid, l, r) + query(root->right, mid + 1, end, l, r);
}
