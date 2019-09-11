#include <iostream>
#include <vector>

using namespace std;

typedef pair<int, int> cols;

pair<int, int> kadane_1d(vector<int> vals, int lim) {
    for (int i = 0; i < vals.size(); i++) cout << vals[i] << " ";
    int best, curr; best = curr = 0;
    int best_length = 0, length = 0;

    for (int i = 0; i < vals.size(); i++) {
        curr = curr + vals[i];
        if (curr > lim) {
            best = curr - vals[i]; best_length = length;
            curr = vals[i];
            length = 1;
        } else {
            best = curr;
            length += 1;
            best_length = length;
        }
    }
    return pair<int, int>(best, best_length);
}

int get_area(int width, int r1, int r2) {
    return width * (r2-r1);
}

pair<int, int> get_best_area_price(vector<vector<int>> matrix, int lim) {
    for (int c = 0; c < matrix[0].size(); c++)
        for (int r = 0; r < matrix.size(); r++)
            matrix[r][c] = matrix[r][c] + ((r > 0) ? matrix[r-1][c] : 0);

    // Add a single row with zeros at the beginning, helps with math
    vector<int> first;
    for (int i = 0; i < matrix[0].size(); i++) first.push_back(0);
    matrix.insert(matrix.begin(), first);

    int best_area = -1, best_price = 100000;

    for (int top = 0; top < matrix.size(); top++){
        for (int bottom = top; bottom < matrix.size(); bottom++) {
            if (top == bottom) continue;

            vector<int> range_sum;
            for (int c = 0; c < matrix[0].size(); c++)
                range_sum.push_back(matrix[bottom][c] - matrix[top][c]);

            auto sol = kadane_1d(range_sum, lim);
            auto sol_area = get_area(sol.second, top, bottom);
            if (sol.first <= 0) continue; // no valid area for these rows

            if (best_area <= sol_area) {
                best_area = sol_area;
                if (best_area == sol_area)
                    if (sol.first < best_price) best_price = sol.first;
                else best_price = sol.first;
            }
        }
    }

    return pair<int, int>(best_area, best_price);
}

vector<int> split_str(string str) {
    vector<int> vals;
    int i = 0, prev_i = 0;
    while (prev_i >= 0) {
        i = str.find(" ", prev_i);
        vals.push_back(stoi(str.substr(prev_i, i-prev_i)));
        cout << str.substr(prev_i, i-prev_i) << endl;
        prev_i = i > 0 ? i + 1 : i;
    }
    return vals;
}

int main(int argc, char const *argv[])
{
    // improve perf
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tests, n, m, k, s;
    string cas;

    cin >> tests;
    for (int t = 0; t < tests; t++) {
        cin >> n >> m >> k;
        vector<vector<int>> matrix;
        for (int ni = 0; ni < n; ni++) {
            vector<int> row;
            for (int mi = 0; mi < m; mi++) {
                cin >> s;
                row.push_back(s);
            }
            matrix.push_back(row);
        }
        pair<int, int> sol = get_best_area_price(matrix, k);
        cout << "Case #" << t+1 << ":" << sol.first << " " << sol.second;
    }
}
