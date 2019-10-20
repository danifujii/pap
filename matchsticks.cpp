#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

#define forr(i,a,b) for(int i=(a); i <(b); i++)
#define forn(i,n) forr(i,0,n)
#define sz(v) (int)(v).size()

typedef pair<int, int> ii;

typedef const int&(*op)(const int&, const int&);  // asociativa, idempotente
template<op op>

struct st{
    vector<vector<int>> t;

    st(const vector<int> &A) { // O(n.logn)
        int n = sz(A);
        int K = 31 - __builtin_clz(n)+1;
        t.assign(K, vector<int>(n));
        forn(i,n) t[0][i] = A[i];
        forr(k,1,K) forn(i, n-(1<<k)+1)
            t[k][i] = op(t[k-1][i], t[k-1][i+(1<<(k-1))]);
    }

    int query(int l, int r) {  // O(1), [l, r)
        int k = 31 - __builtin_clz(r-l);
        return op(t[k][l], t[k][r-(1<<k)]);
    }
};


void solve(const vector<int> & burns, const vector<ii> & queries) {
    st<max> max_rmq(burns); st<min> min_rmq(burns);
    for (ii query: queries) {
        double l = min(query.first, query.second); double r = max(query.first, query.second)+1;
        double range_min = min_rmq.query(l, r);
        double range_max = max_rmq.query(l, r);

        double left_max = 0, right_max = 0;
        if (l > 0) left_max = max_rmq.query(0, l);
        if (r < burns.size()) right_max = max_rmq.query(r, burns.size());
        double burn_two = range_min + max(left_max, right_max);

        // Range max gets burnt by the minimum, and speeds up the burn left
        double range_max_remaining = range_max - range_min;
        cout << max(burn_two, range_min + range_max_remaining/2) << "\n";
    }
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);

    cout << fixed;
	cout << setprecision(1);//Para devolver un solo decimal

    int n;
    int b;
    cin >> n;

    vector<int> burns(n);
    for (int i = 0; i < n; ++i) {
        cin >> b;
        burns[i] = b;
    }

    int q, l, r;
    cin >> q;
    vector<ii> queries(q);
    for (int i = 0; i < q; ++i) {
        cin >> l >> r;
        queries[i] = ii(l, r);
    }
    solve(burns, queries);
}