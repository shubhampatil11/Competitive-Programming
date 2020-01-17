template<typename T>
class treapset {
  private:
    struct item;
    using pitem = item*;

    mt19937_64 rnd;
    bool is_sorted_;
    bool stop_;
    pitem root_;

    using priority_t = mt19937_64::result_type;

    priority_t next_priority() {
        priority_t priority = rnd();
        return priority;
    }

    void regenerate_priorities_recursive(vector<int> &new_priors, pitem& t, int l, int r) {
        if (!t) return;
        t->priority = new_priors[r - 1];
        regenerate_priorities_recursive(new_priors, t->l, l, l + cnt(t->l));
        regenerate_priorities_recursive(new_priors, t->r, l + cnt(t->l), r - 1);
    }

    void regenerate_priorities() {
        int sz = size();
        vector<int> new_priors(sz);
        for (int i = 0; i < sz; i++) new_priors[i] = next_priority();
        sort(new_priors.begin(), new_priors.end());
        for (int i = 0; i < sz; i++) new_priors[i] += i;
        regenerate_priorities_recursive(new_priors, root_, 0, sz);
    }

    struct item {
        priority_t priority;
        int cnt, rev;
        T key, lazy, fsum;
        pitem l, r;

        // key : value of node
        // fsum : value of subtree
        // lazy : lazy propogation value
        // cnt : size of subtree
        // rev : denotes if range needs to be reversed
        item(T x, priority_t p) {
            lazy = 0LL * x;
            key = fsum = x;
            cnt = 1;
            rev = 0;
            l = r = nullptr;
            priority = p;
        }
    };

    int cnt(pitem it) {
        return it ? it->cnt : 0;
    }

    void upd_cnt(pitem it) {
        if (it) it->cnt = cnt(it->l) + cnt(it->r) + 1;
    }

    void upd_sum(pitem it) {
        if (it) {
            it->fsum = it->key;
            if (it->l) it->fsum = it->fsum + it->l->fsum;
            if (it->r) it->fsum = it->fsum + it->r->fsum;
        }
    }

    void update(pitem t, T lazy, int rev) {
        if (!t) return;
        t->lazy = t->lazy + lazy;
        t->rev = t->rev ^ rev;
        t->key = t->key + lazy;
        t->fsum = t->fsum + cnt(t) * lazy;
    }

    void push (pitem t) {
	 	if (!t || (t->lazy == 0*T() && t->rev == 0)) return;
		update (t->l, t->lazy, t->rev);
		update (t->r, t->lazy, t->rev);
		if(t->rev) swap(t->l,t->r);
		t->lazy = 0*T();
		t->rev = 0;
	}

    void merge(pitem& t, pitem l, pitem r) {
        push(l);
        push(r);
        if (!l || !r)
            t = l ? l : r;
        else if (l->priority > r->priority) {
            merge(l->r, l->r, r);
            t = l;
        } else {
            merge(r->l, l, r->l);
            t = r;
        }
        upd_cnt(t);
        upd_sum(t);
    }

    void split(pitem t, pitem &l, pitem &r, int index) { // split at position
        if (!t) return void( l = r = 0 );
        push(t);
        if (index <= cnt(t->l)) {
            split(t->l, l, t->l, index);
            r = t;
        } else {
            split(t->r, t->r, r, index - 1 - cnt(t->l));
            l = t;
        }
        upd_cnt(t);
        upd_sum(t);
    }

    void split_at(pitem t, pitem &l, pitem &r, T &key, bool &eq) { // split by key
        if (!t) return void(l = r = 0);
        push(t);
        if (key == t->key) {
            eq = true;
            return;
        } else if (key < t->key) {
            split_at(t->l, l, t->l, key, eq);
            if (!eq) r = t;
        } else {
            split_at(t->r, t->r, r, key, eq);
            if (!eq) l = t;
        }
        if (!eq) upd_cnt(t);
        upd_sum(t);
    }

    void insert(pitem &t, pitem it, int index) { // insert at position
        push(t);
        if (!t)
            t = it;
        else if (it->priority == t->priority) {
            stop_ = true;
            regenerate_priorities();
        } else if (it->priority > t->priority) {
            split(t, it->l, it->r, index);
            t = it;
        } else if (index <= cnt(t->l))
            insert(t->l, it, index);
        else
            insert(t->r, it, index - cnt(t->l) - 1 );
        if (stop_) return;
        upd_cnt(t);
        upd_sum(t);
    }

    void insert_at(pitem &t, pitem it, bool &eq) { // insert by key
        push(t);
        if (!t)
            t = it;
        else if (it->key == t->key) {
            eq = true;
            return;
        } else if (it->priority == t->priority) {
            stop_ = true;
            regenerate_priorities();
        } else if (it->priority > t->priority) {
            split_at(t, it->l, it->r, it->key, eq);
            if (!eq) t = it;
        } else if (it->key < t->key)
            insert_at(t->l, it, eq);
        else
            insert_at(t->r, it, eq);
        if (stop_) return;
        if (!eq) upd_cnt(t);
        upd_sum(t);
    }

    void erase(pitem &t, int index) {
        push(t);
        if (cnt(t->l) == index)
            merge(t, t->l, t->r);
        else if (index < cnt(t->l))
            erase(t->l, index);
        else
            erase(t->r, index - cnt(t->l) - 1 );
        upd_cnt(t);
        upd_sum(t);
    }

    void erase_at(pitem &t, T key, bool &found) {
        push(t);
        if (key == t->key) {
            merge(t, t->l, t->r);
            found = true;
        } else if (key < t->key)
            erase_at(t->l, key, found);
        else
            erase_at(t->r, key, found);
        upd_cnt(t);
        upd_sum(t);
    }

    T get(pitem t, int index) {
        push(t);
        if (index < cnt(t->l))
            return get(t->l, index);
        else if (index > cnt(t->l))
            return get(t->r, index - cnt(t->l) - 1);
        return t->key;
    }

    pair<T, int> lower_bound(pitem t, T key, int index) {
        push(t);
        if (!t) return {T(), size()};
        if (key == t->key) return {key, index + cnt(t->l)};
        if (key < t->key) {
            pair<T, int> ret = lower_bound(t->l, key, index);
            if (ret.second == size()) ret = {t->key, index + cnt(t->l)};
            return ret;
        }
        return lower_bound(t->r, key, index + 1 + cnt(t->l));
    }

    pair<T, int> upper_bound(pitem t, T key, int index) {
        push(t);
        if (!t) return {T(), size()};
        if (key < t->key) {
            pair<T, int> ret = upper_bound(t->l, key, index);
            if (ret.second == size()) ret = {t->key, index + cnt(t->l)};
            return ret;
        }
        return upper_bound(t->r, key, index + 1 + cnt(t->l));
    }

    void range_update(pitem &t, int l, int r, T lazy) {
        pitem l1, r1;
        split(t, l1, r1, r + 1);
        pitem l2, r2;
        split(l1, l2, r2, l);
        update(r2, lazy, 0);
        pitem t2;
        merge(t2, l2, r2);
        merge(t, t2, r1);
    }

    void reverse(pitem &t, int l, int r) {
        pitem l1, r1;
        split(t, l1, r1, r + 1);
        pitem l2, r2;
        split(l1, l2, r2, l);
        update(r2, 0*T(), 1);
        pitem t2;
        merge(t2, l2, r2);
        merge(t, t2, r1);
    }

    void move(pitem &t, int left, int right, int shift) {
        // [l, r) becomes [l+shift, r+shift)
        if(shift == 0) return;
        int l = left + min(0, shift);
        int r = right + max(0, shift);
        int m = (shift > 0) ? right : left;
        pitem prefix, tmp;
        split(t, prefix, tmp, l);
        pitem suffix, middle;
        split(tmp, middle, suffix, r - l);
        pitem mid_prefix, mid_suffix;
        split(middle, mid_prefix, mid_suffix, m - l);
        merge(middle, mid_suffix, mid_prefix);
        merge(tmp, middle, suffix);
        merge(t, prefix, tmp);
    }

    T get_sum(pitem &t, int l, int r) {
        pitem l1, r1;
        split(t, l1, r1, r + 1);
        pitem l2, r2;
        split(l1, l2, r2, l);
        T ret = r2->fsum;
        pitem t2;
        merge(t2, l2, r2);
        merge(t, t2, r1);
        return ret;
    }

  public:
    treapset() : is_sorted_(true), stop_(false), root_(nullptr) {
        rnd = mt19937_64(chrono::steady_clock::now().time_since_epoch().count());
    }

    int size() {
        return cnt(root_);
    }
    bool empty() {
        return (cnt(root_) == 0);
    }
    bool is_sorted() {
        return is_sorted_;
    }

    void srand(mt19937_64::result_type seed) {
        // optional
        rnd.seed(seed);
    }

    // return true if inserted
    bool insert(T x) {
        if (!is_sorted_) return false;
        bool eq = false;
        pitem it = new item(x, next_priority());
        stop_ = false;
        insert_at(root_, it, eq);
        while (stop_) {
            stop_ = false;
            eq = false;
            insert_at(root_, it, eq);
        }
        return !eq;
    }

    void insert_at(int pos, T x) {
        if (pos > size()) return;
        pitem it = new item(x, next_priority());
        stop_ = false;
        insert(root_, it, pos);
        while (stop_) {
            stop_ = false;
            insert(root_, it, pos);
        }
        if (pos > 0 && is_sorted_) {
            if (get(root_, pos - 1) >= get(root_, pos)) is_sorted_ = false;
        }
        if (pos < size() - 1 && is_sorted_) {
            if (get(root_, pos) >= get(root_, pos + 1)) is_sorted_ = false;
        }
    }

    bool erase(T x) {
        if (!is_sorted_) return false;
        bool found = false;
        erase_at(root_, x, found);
        return found;
    }

    void erase_at(int pos) {
        if (pos >= size()) return;
        erase(root_, pos);
    }

    int get_index(T key) {
        if (!is_sorted_) return size();
        pitem t = root_;
        int index = 0;
        while (t && t->key != key) {
            if (t->key > key)
                t = t->l;
            else {
                index += cnt(t->l) + 1;
                t = t->r;
            }
        }
        if (!t) return size();
        index += cnt(t->l);
        return index;
    }

    T operator[](int index) {
        return get(root_, index);
    }

    pair<T, int> lower_bound(T x) {
        if (!is_sorted_) return {T(), size()};
        return lower_bound(root_, x, 0);
    }

    pair<T, int> upper_bound(T x) {
        if (!is_sorted_) return {T(), size()};
        return upper_bound(root_, x, 0);
    }

    void range_update(int left, int right, T x) {
        left = max(left, 0);
        right = min(right, size() - 1);
        range_update(root_, left, right, x);
        if (left > 0 && is_sorted_) {
            if (get(root_, left - 1) >= get(root_, left)) is_sorted_ = false;
        }
        if (right < size() - 1 && is_sorted_) {
            if (get(root_, right) >= get(root_, right + 1)) is_sorted_ = false;
        }
    }

    void reverse(int left, int right) {
        left = max(left, 0);
        right = min(right, size() - 1);
        reverse(root_, left, right);
        if (left != right) is_sorted_ = false;
    }

    void move(int left, int right, int shift) {
        move(root_, left, right + 1, shift);
    }

    T get_sum(int left, int right) {
        return get_sum(root_, left, right);
    }
}; // class treap_set

template<typename T>
ostream& operator << (ostream& os, treapset<T> &v) {
    for (int i = 0; i < v.size(); i++) os << v[i] << ' ';
    return os;
}
