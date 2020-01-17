struct LCT {
    bool tr;
    struct node {
        node *c[2], *p;
        bool flip ;
        int label, depth;
        int lz, sz ;
        node() {
            c[0] = c[1] = p = 0;
            flip = lz = 0;
            sz = 1;
        }
    };

    node *T;

    LCT(int n, bool tree = 0) { // 0 - Indexed
        T = new node[n];
        tr = tree;
        for(int i = 0; i < n; i++) {
            T[i].label = i;
        }
    }

    void push(node *x) {
        if(x->flip) {
            swap(x->c[0], x->c[1]);
            x->flip = 0;
            for(int i = 0; i < 2; i++) {
                if(x->c[i]) {
                    x->c[i]->flip ^= 1;
                }
            }
        }
        for(int i = 0; i < 2; i++) {
            if(x->c[i]) {
                x->c[i]->lz += x->lz;
            }
        }
        x->sz += x->lz;
        x->lz = 0;
    }

    void upd(node* x, int val = 0) {
        x->depth = 1;
        for(int i = 0; i < 2; i++) {
            if(x->c[i]) {
                x->depth += x->c[i]->depth;
            }
        }
    }

    bool is_root(node *x) {
        return !x->p || (x->p->c[0] != x && x->p->c[1] != x);
    }

    bool dir(node *x) {
        return x->p->c[1] == x;
    }

    void setc(node *x, node *y, int f) {
        x->c[f] = y;
        if(y) {
            y->p = x;
        }
    }

    void rot(node *x) {
        node *y = x->p;
        bool d = dir(x);
        if(!is_root(y)) {
            setc(y->p, x, dir(y));
        }
        else {
            x->p = y->p;
        }
        setc(y, x->c[!d], d);
        setc(x, y, !d);
        upd(y);
    }

    void splay(node *x) {
        node *q = x;
        stack<node*> ss;
        while(1) {
            ss.push(q);
            if(is_root(q)) {
                break;
            }
            q = q->p;
        }
        while(!ss.empty()) {
            push(ss.top());
            ss.pop();
        }
        while(!is_root(x)) {
            if(is_root(x->p)) {
                rot(x);
            }
            else if(dir(x) == dir(x->p)) {
                rot(x->p);
                rot(x);
            }
            else {
                rot(x);
                rot(x);
            }
        }
        upd(x);
    }

    node *access(node *x) {
        node *q = 0, *tt = x;
        for(; x; x = x->p) {
            splay(x);
            x->c[1] = q;
            upd(q = x);
        }
        splay(tt);
        return q;
    }

    node *root(node *x) {
        access(x);
        while(x->c[0]) {
            x = x->c[0];
        }
        access(x);
        return x;
    }

    int lca(int u, int v) {
        access(&T[u]);
        return access(&T[v])->label;
    }

    void mkroot(node *x) {
        access(x);
        if(!tr) {
            x->flip ^= 1;
        }
        push(x);
    }

    bool conn(int u, int v) {
        return root( &T[u] ) == root(&T[v]);
    }

    int height(int u) {
        node *x = &T[u];
        access(x);
        return x->depth - 1;
    }

    int size(int u) {
        node *x = &T[u];
        access(x);
        push(x);
        return x->sz;
    }

    void link(int u, int v) {
        node *x = &T[u], *y = &T[v];
        access(y);
        mkroot(x);
        setc(y, x, 1) ;
        y->sz += x->sz + x->lz;
        if(y->c[0]) {
            y->c[0]->lz += x->sz + x->lz ;
        }
    }

    void cut(int u) {
        node *x = &T[u];
        access(x);
        push(x) ;
        x->c[0]->lz -= x->sz + x->lz ;
        if(x->c[0]) {
            x->c[0]->p = 0;
        }
        x->c[0] = 0;
    }

    void cut(int u, int v) {
        mkroot(&T[v]);
        cut(u);
    }
};



