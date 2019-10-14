#include <iostream>
#include <vector>
using namespace std;

typedef vector<short> vs;
typedef unsigned long long ull;

int n, m, k;

int elem_at_idx(int st, int idx) {
    int m = 3 << (idx*2);
    return (st & m) >> (idx*2);
}

int insert_elem(int st, int elem) {
    st = st << 2;
    st = st & ((1 << (n+1)*2) - 1);
    return st | elem;
}

bool valid(int k, int r, int c, int st) {
    if (r > 0) {
        int upward_elem = elem_at_idx(st, 0);
        if (k == upward_elem) return false;
    }
    if (c > 0) {
        int leftward_elem = elem_at_idx(st, n-1);
        if (k == leftward_elem) return false;
    }
    return true;
}

ull amounts(const vector<vs> & mat, int r, int c, int st) {
    if (r == 0 && c == m) return 1;

    short cell = mat[r][c];
    if (cell == -1) {
        bool last_elem = r == n-1;
        ull sum = 0;
        for (int ki = 0; ki < k; ++ki) {
            if (valid(ki, r, c, st)) {
                int new_st = insert_elem(st, ki);
                sum += amounts(mat, last_elem?0:r+1, last_elem?c+1:c, new_st);
            }
        }
        return sum;
    } else {  // Set value
        if (valid(cell, r, c, st)) {
            st = insert_elem(st, cell);
            return amounts(mat, r < n-1 ? r+1 : 0, r < n-1 ? c : c+1, st);
        } else return 0;
    }
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);

    char cell;
    cin >> n >> m >> k;
    vector<vs> mat(n);

    for (int r = 0; r < n; ++r) {
        vs row(m);
        for (int c = 0; c < m; ++c) {
            cin >> cell;
            if (cell == '-')
                row[c] = -1;
            else row[c] = (cell - '0')-1;
        }
        mat[r] = row;
    }
    cout << amounts(mat, 0, 0, 0);
}