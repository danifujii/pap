#include <iostream>
#include <set>

using namespace std;

typedef pair<int, int> ii;
typedef set<ii> sii;

bool b1[1000][1000];
bool b2[1000][1000];

bool equivalent(sii items_b1, sii items_b2) {
    // Matrices intialization
    for (int c = 0; c < 1000; c++) for (int r = 0; r < 1000; r++) {
        if (items_b1.find(ii(r, c)) != items_b1.end()) {
            b1[c][r] = true;
        } else b1[c][r] = false;
        if (items_b2.find(ii(r, c)) != items_b2.end()) {
            b2[c][r] = true;
        } else b2[c][r] = false;
    }
}

int main()
{
    // improve perf
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tests, w, h, pieces, x, y;
    cin >> tests;
    while (tests) {
        cin >> w >> h >> pieces;

        sii items_b1;
        for (int p = 0; p < pieces; p++) {
            cin >> x >> y;
            items_b1.insert(ii(x, y));
        }

        sii items_b2;
        for (int p = 0; p < pieces; p++) {
            cin >> x >> y;
            items_b2.insert(ii(x, y));
        }

        if (equivalent(items_b1, items_b2)) {
            cout << "YES\n";
        } else cout << "NO\n";

        tests--;
    }
}