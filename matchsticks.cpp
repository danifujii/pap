#include <iostream>
#include <vector>

using namespace std;

typedef pair<int, int> ii;

void solve(const vector<int> & burns, const vector<ii> & queries) {

}

int main() {
    int n;
    long b;
    cin >> n;

    vector<long> burns(n);
    for (int i = 0; i < n; ++i) {
        cin >> b;
        burns[i] = b;
    }

    int q, l, r;
    cin >> q;
    vector<ii> queries(q);
    for (int i = 0; i < q; ++i) {
        cin >> l >> r;
        queries.push_back(ii(l, r));
    }
}