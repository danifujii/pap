#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::begin;
using std::end;

// EDIT ME - segment tree and additional params
// data type of each element of the tree
using data = int;
// neutral value, i.e. op(a, neut) = op(neut, a) = a
const data neut = 0;

// additional problem-specific parameters
int b, p, l, n;
std::vector<data> v;

// tree binary operation
const auto left_op = [](data& a, data& b) {
    return a != -1 && a != neut ? a : b;
};

const auto right_op = [](data& a, data& b) -> data {
    return b != -1 && b != neut ? b : a;
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

inline void update(std::vector<data> & st, int i, data(* op)(data&, data&)) {
    auto l = st[lchild(i)];
    auto r = st[rchild(i)];
    auto val = op(l, r);
    st[i] = val;
}

// SEGMENT TREE - QUERY
data query(const std::vector<data> & st, int l, int r, int idx, int i, int j, data(* op)(data&, data&)) {
  // return node if fully contained in range
  if (l <= i && j <= r) return st[idx];
  // return neutral value if disjoint
  if (r <= i || l >= j) return neut;
  // combine children if partially contained
  int m = (i + j) / 2;

  data left_data = query(st, l, r, lchild(idx), i, m, op);
  data right_data = query(st, l, r, rchild(idx), m, j, op);
  return op(left_data, right_data);
}

data query(const std::vector<data> & st, int l, int r, data(* op)(data&, data&)) {
  return query(st, l, r, 1, 0, leaf_offset(st), op);
}

// SEGMENT TREE - SET
void set(std::vector<data> & st, int i, const data& v, data(* op)(data&, data&)) {
  // set on leaf node
  st[i += leaf_offset(st)] = v;
  // propagate upwards
  while (i = parent(i)) update(st, i, op);
}

// SEGMENT TREE - INITIALIZATION
template<class Iter>
void init_segtree(std::vector<data> & st, Iter begin, Iter end, data(* op)(data&, data&)) {
    size_t size = nextPow2(std::distance(begin, end));
    // allocate space for tree
    st.clear(); st.resize(size << 1, neut);
    // load values into tree
    std::copy(begin, end, st.begin() + size);
    // compute upper levels
    while (size = parent(size))
        for (size_t i = size; i < 2 * size; i++)
            update(st, i, op);
}

int main() {
    std::ios::sync_with_stdio(0); cin.tie(0);
    int s, b, ql, qr;

    while (true) {
        cin >> s >> b;
        if (s == 0) break;

        std::vector<int> soldiers(s);
        for (int i = 0; i < s; ++i) {
            soldiers[i] = i + 1;
        }
        std::vector<data> left_st, right_st;
        init_segtree(left_st, soldiers.begin(), soldiers.end(), left_op);
        init_segtree(right_st, soldiers.begin(), soldiers.end(), right_op);

        for (int i = 0; i < b; ++i) {
            cin >> ql >> qr;
            for (int j = ql-1; j < qr; j++) {
                set(left_st, j, -1, left_op);
                set(right_st, j, -1, right_op);
            }
            auto left = query(right_st, 0, ql, right_op);
            auto right = query(left_st, qr, soldiers.size(), left_op);
            if (left != -1 && left != neut) cout << left;
            else cout << "*";
            cout << " ";
            if (right != -1 && right != neut) cout << right;
            else cout << "*";
            cout << "\n";
        }
        cout << "-\n";
    }
}