#include <iostream>
#include <vector>
using namespace std;

typedef vector<int> vi;
typedef pair<int, int> query;

int x = 100000;

int bin_search_lower(const vi & zs, int val) {
    int low = 0, high = zs.size() - 1, mid, idx = -1;
    while (low <= high) {
        mid = (high + low) / 2;
        if (zs[mid] >= val) {
            idx = mid;
            high = mid - 1;
        } else low = mid + 1;
    }
    return idx;
}

int bin_search_upper(const vi & zs, int val) {
    int low = 0, high = zs.size() - 1, mid, idx = -1;
    while (low <= high) {
        mid = (high + low) / 2;
        if (zs[mid] > val)
            high = mid - 1;
        else {
            idx = mid;
            low = mid + 1;
        }
    }
    return idx;
}

void solve_query(const vector<vi> & zones, const query & q) {
    int biggest_size = -1;
    for (int r = 0; r < zones.size(); r++) {
        int lower = bin_search_lower(zones[r], q.first);
        int upper = bin_search_upper(zones[r], q.second);
        if (lower > upper || lower == -1 || upper == -1) continue; // no possible answer
        while (upper >= lower) {
            int size = upper-lower;
            if (size < biggest_size) break;
            if (r+size >= zones.size()) {
                upper -= 1;
                continue; // square would be out of bounds
            }

            int bottom_right = zones[r+size][upper];
            if (bottom_right <= q.second) {
                biggest_size = max(biggest_size, size+1);
                break;
            } else upper -= 1;
        }
    }
    if (biggest_size > 0) {
        cout << biggest_size << "\n";
    } else cout << "0\n";
}

void solve(const vector<vi> & zones, const vector<query> & qs) {
    for (query q: qs) solve_query(zones, q);
}

int main()
{
    // improve perf
    ios::sync_with_stdio(0); cin.tie(0);

    int n, m, q, l, u, z;
    while (true) {
        cin >> n >> m;
        if (n == 0) break;

        vector<vi> zones(n, vi(m));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                cin >> z;
                zones[i][j] = z;
            }
        }
        vector<query> queries;
        cin >> q;
        for (int i = 0; i < q; i++) {
            cin >> l >> u;
            queries.push_back(query(l, u));
        }
        solve(zones, queries);
        cout << "-\n";
    }
}