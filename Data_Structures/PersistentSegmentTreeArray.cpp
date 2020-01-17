int NEXT_FREE_INDEX = 0;
int L[N], R[N], s[N];

int build(int start, int end) {
    int ID = NEXT_FREE_INDEX++;
    if(start == end) {
        s[ID] = 0;
        return ID;
    }
    int mid = (start + end) >> 1;
    L[ID] = build(start, mid);
    R[ID] = build(mid + 1, end);
    return ID;
}

int update(int id, int start, int end, int p, int val) {
    int ID =  NEXT_FREE_INDEX++;
    if(start == end) {
        s[ID] = s[id] + val;
        return ID;
    }
    int mid = (start + end) >> 1;
    L[ID] = L[id], R[ID] = R[id];
    if(p <= mid) {
        L[ID] = update(L[id], start, mid, p, val);
    }
    else {
        R[ID] = update(R[id], mid + 1, end, p, val);
    }
    s[ID] = s[L[ID]] + s[R[ID]];
    return ID;
}

int query(int id, int start, int end, int l, int r) {
	if(start >= l && end <= r) {
        return s[id];
	}
	if(end < l || start > r) {
        return 0;
	}
	int mid = (start + end) >> 1;
	return query(L[id], start, mid, l, r) + query(R[id], mid + 1, end, l, r);
}
