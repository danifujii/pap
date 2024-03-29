#include <iostream>
#include <vector>

using namespace std;

typedef long long ll;

pair<ll, ll> kadane_1d(vector<ll> vals, ll lim, int top, int bottom) {
    ll best, curr; best = -1; curr = -1;
    int best_length = 0, length = 0;

    for (int i = 0; i < vals.size(); i++) {
        if (length >= best_length) {
            if (length == best_length && best >= 0 && curr >= 0)
                best = min(best, curr);
            else best = curr;
            best_length = length;
        }
        if (curr > 0)
            curr = curr + vals[i];
        else curr = vals[i];
        length += 1;
        if (curr > lim) {
            // try fixing it by removing start elements
            while (curr > lim && length > 0) {
                auto remove = vals[i-length+1];
                curr -= remove;
                length -= 1;
            }
        }
    }
    // Check for last element
    if (length >= best_length) {
        if (length == best_length && best >= 0 && curr >= 0)
            best = min(best, curr);
        else best = curr;
        best_length = length;
    }
    return pair<ll, ll>(best, best_length);
}

int get_area(int width, int r1, int r2) {
    return width * (r2-r1);
}

pair<ll, ll> get_best_area_price(vector<vector<ll>> matrix, ll lim) {
    for (int c = 0; c < matrix[0].size(); c++)
        for (int r = 0; r < matrix.size(); r++)
            matrix[r][c] = matrix[r][c] + ((r > 0) ? matrix[r-1][c] : 0);

    // Add a single row with zeros at the beginning, helps with math
    vector<ll> first;
    for (int i = 0; i < matrix[0].size(); i++) first.push_back(0);
    matrix.insert(matrix.begin(), first);

    ll best_area = -1, best_price = 100000000;

    for (int top = 0; top < matrix.size(); top++){
        for (int bottom = top; bottom < matrix.size(); bottom++) {
            vector<ll> range_sum;
            for (int c = 0; c < matrix[0].size(); c++)
                range_sum.push_back(matrix[bottom][c] - matrix[top][c]);

            auto sol = kadane_1d(range_sum, lim, top, bottom);
            auto sol_area = get_area(sol.second, top, bottom);
            if (sol.first < 0) continue; // no valid area for these rows

            if (best_area <= sol_area) {
                if (best_area == sol_area) {  // best yet is the same we found, then we only update if the price is better
                    if (sol.first < best_price) best_price = sol.first;
                } else best_price = sol.first;
                best_area = sol_area;
            }
        }
    }

    return pair<long, long>(best_area, best_price);
}

int main(int argc, char const *argv[])
{
    // improve perf
    ios::sync_with_stdio(0);
    cin.tie(0);

    ll k;
    int tests, n, m, s;
    string cas;

    cin >> tests;
    for (int t = 0; t < tests; t++) {
        cin >> n >> m >> k;
        vector<vector<ll>> matrix;
        for (int ni = 0; ni < n; ni++) {
            vector<ll> row;
            for (int mi = 0; mi < m; mi++) {
                cin >> s;
                row.push_back(s);
            }
            matrix.push_back(row);
        }
        pair<ll, ll> sol = get_best_area_price(matrix, k);
        if (sol.first <= 0)
            cout << "Case #" << t+1 << ": " << 0 << " " << 0 << "\n";
        else cout << "Case #" << t+1 << ": " << sol.first << " " << sol.second << "\n";
    }
}
