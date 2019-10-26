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

std::vector<data> st;

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

// SEGMENT TREE - QUERY
data query(int l, int r, int idx, int i, int j) {
  // return node if fully contained in range
  if (l <= i && j <= r) return st[idx];
  // return neutral value if disjoint
  if (r <= i || l >= j) return neut;
  // combine children if partially contained
  int m = (i + j) / 2;

  data left_data = query(l, r, lchild(idx), i, m);
  data right_data = query(l, r, rchild(idx), m, j);
  return op(left_data, right_data);
}

data query(int l, int r) {
  return query(l, r, 1, 0, leaf_offset(st));
}

// SEGMENT TREE - SET
void set(int i, const data& v) {
  // set on leaf node
  st[i += leaf_offset(st)] = v;
  // propagate upwards
  while (i = parent(i)) update(i);
}

// SEGMENT TREE - INITIALIZATION
template<class Iter>
void init_segtree(Iter begin, Iter end) {
    size_t size = nextPow2(std::distance(begin, end));
    // allocate space for tree
    st.clear(); st.resize(size << 1, neut);
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
            } else if (s == "F") { // set to Bucaneer (1)

            } else if (s == "") { // set to Barbary (0)

            } else if (s == "I") { // invert in range

            }
        }
    }
}