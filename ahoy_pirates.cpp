#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::begin;
using std::end;

// data type of each element of the tree
using data = int;
// neutral value, i.e. op(a, neut) = op(neut, a) = a
const data neut = 0;
const data lazy_neut = -2;

std::vector<data> st;
std::vector<data> lazy;

// tree binary operation
const auto op = [](const data& a, const data& b) -> data {
    return a + b;
};

// SEGMENT TREE - HELPER FUNCTIONS
int nextPow2(int n) {
    int r = 1;
    while(r < n) r = r << 1;
    return r;
}

inline int leaf_offset(const std::vector<data> & st) {
  return st.size() >> 1;
}

inline int lchild(int i) {
    return i << 1;
}

inline int rchild(int i) {
    return lchild(i) + 1;
}

inline int parent(int i) {
    return i >> 1;
}

inline void update(int i) {
    auto l = st[lchild(i)];
    auto r = st[rchild(i)];
    auto val = op(l, r);
    st[i] = val;
}

int combine_lazy(int new_lazy_op, int n) {
    if (lazy[n] == lazy_neut) return new_lazy_op;  // nothing to combine
    if (new_lazy_op == -1) {  // flip, then depends on what was there
        if (lazy[n] == -1) return lazy_neut;  // other flip, then nothing
        else return (lazy[n] == 1) ? 0 : 1;  // flip existing value
    }
    else return new_lazy_op;  // new op wins against old set one
}

void apply_lazy(int n, int nl, int nr, int lazy_op) {
    if (lazy_op == -1) {
        if (n < leaf_offset(st))
            st[n] = (nr-nl)-st[n];  // flip the amount we currently have
        else st[n] = (st[n] == 1 ? 0 : 1); // its a leaf, just invert
    } else st[n] = (nr-nl)*lazy_op;

    if (n < leaf_offset(st)) {
        lazy[lchild(n)] = combine_lazy(lazy_op, lchild(n));
        lazy[rchild(n)] = combine_lazy(lazy_op, rchild(n));
    }
}

// SEGMENT TREE - QUERY
data query(int l, int r, int idx, int i, int j) {
    // apply lazy updates if necessary
    if (lazy[idx] != lazy_neut) {
        apply_lazy(idx, i, j, lazy[idx]);
        lazy[idx] = lazy_neut;
    }

    if (r <= i || l >= j) return neut;  // return neutral value if disjoint

    int val = st[idx];
    if (l <= i && j <= r) return val;  // return node if fully contained in range
    int m = (i + j) / 2; // combine children if partially contained

    data left_data = query(l, r, lchild(idx), i, m);
    data right_data = query(l, r, rchild(idx), m, j);
    return op(left_data, right_data);
}

data query(int l, int r) {
    return query(l, r, 1, 0, leaf_offset(st));
}

// SEGMENT TREE - SET
void set_node(int i, const data& v) {
    // set on leaf node
    data current_leaf = st[i];  // i is already correct leaf index
    if (v == -1)
        st[i] = (current_leaf == 1 ? 0 : 1);
    else st[i] = v;
}

void set_range(int n, int nl, int nr, int l, int r, const data & v) {
    // apply lazy updates if necessary
    if (lazy[n] != lazy_neut) {
        apply_lazy(n, nl, nr, lazy[n]);
        lazy[n] = lazy_neut;
    }

    if (r <= nl || l >= nr) return;  // out of range

    if (l <= nl && nr <= r) { // completely in range
        apply_lazy(n, nl, nr, v);
        return;
    }

    int m = (nl + nr) / 2;
    set_range(lchild(n), nl, m, l, r, v);
    set_range(rchild(n), m, nr, l, r, v);
    update(n);
}

void set_range(int l, int r, const data & v) {
    set_range(1, 0, leaf_offset(st), l, r, v);
}

// SEGMENT TREE - INITIALIZATION
template<class Iter>
void init_segtree(Iter begin, Iter end) {
    size_t size = nextPow2(std::distance(begin, end));
    // allocate space for tree
    st.clear(); st.resize(size << 1, neut);
    lazy.clear(); lazy.resize(size << 1, lazy_neut);

    // load values into tree
    std::copy(begin, end, st.begin() + size);
    // compute upper levels
    while (size = parent(size))
        for (size_t i = size; i < 2 * size; i++)
            update(i);
}

int main() {
    std::ios::sync_with_stdio(0); cin.tie(0);

    int t, m, c, q, l, r;
    std::string s;
    cin >> t;
    for (int ti = 0; ti < t; ti++) {
        cin >> m;
        std::vector<int> ps;
        for (int i = 0; i < m; i++) {
            cin >> c >> s;
            for (int j = 0; j < c; j++) {
                for (char si: s) ps.push_back(si == '1' ? 1 : 0);
            }
        }
        init_segtree(ps.begin(), ps.end());
        cout << "Case " << ti+1 << ":\n";

        // Queries/ops time
        cin >> q;
        int qn = 0;
        for (int i = 0; i < q; i++) {
            cin >> s >> l >> r;
            if (s == "S") {
                qn++;
                cout << "Q" << qn << ": " << query(l, r+1) << "\n";
            }
            else if (s == "F") set_range(l, r+1, 1); // set to Bucaneer (1)
            else if (s == "E") set_range(l, r+1, 0);  // set to Barbary (0)
            else if (s == "I") set_range(l, r+1, -1);  // invert in range
        }
    }
}