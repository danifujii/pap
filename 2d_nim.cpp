#include <iostream>
#include <set>
#include <vector>

using namespace std;

typedef pair<int, int> ii;
typedef set<ii> sii;

bool b1[1000][1000];
bool b2[1000][1000];

ii left_top_corner, left_bottom_corner, right_bottom_corner, right_top_corner;
sii visited_b1, visited_b2;

sii neighbors(ii coord, int max_width, int max_height, sii nodes) {
    sii rv;

    ii left = ii(coord.first-1, coord.second);
    if (coord.first > 0 && nodes.find(left) != nodes.end()) rv.insert(left);

    ii right = ii(coord.first+1, coord.second);
    if (coord.first < max_width && nodes.find(right) != nodes.end()) rv.insert(right);

    ii bottom = ii(coord.first, coord.second-1);
    if (coord.second > 0 && nodes.find(bottom) != nodes.end()) rv.insert(bottom);

    ii top = ii(coord.first, coord.second+1);
    if (coord.second < max_height && nodes.find(top) != nodes.end()) rv.insert(top);

    return rv;
}

void update_bounds(ii coord) {
    if (coord.first < left_top_corner.first) left_top_corner.first = coord.first;
    if (coord.second < left_top_corner.second) left_top_corner.second = coord.second;
    if (coord.first < left_bottom_corner.first) left_bottom_corner.first = coord.first;
    if (coord.second > left_bottom_corner.second) left_bottom_corner.second = coord.second;
    if (coord.first > right_bottom_corner.first) right_bottom_corner.first = coord.first;
    if (coord.second > right_bottom_corner.second) right_bottom_corner.second = coord.second;
    if (coord.first > right_top_corner.first) right_top_corner.first = coord.first;
    if (coord.second < right_top_corner.second) right_top_corner.second = coord.second;
}

void reset_bounds() {
    left_top_corner.first = 1005; left_bottom_corner.first = 1005; right_bottom_corner.first = -1; right_top_corner.first = -1;
    left_top_corner.second = 1005; left_bottom_corner.second = -1; right_bottom_corner.second = -1; right_top_corner.second = 1005;
}

void dfs(ii root, sii* visited, sii* connected_component, int max_width, int max_height, sii nodes) {
    if (visited->find(root) != visited->end()) return;

    visited->insert(root);
    connected_component->insert(root);
    update_bounds(root);
    for (ii n: neighbors(root, max_width, max_height, nodes)) {
        dfs(n, visited, connected_component, max_width, max_height, nodes);
    }
}

string get_matrix_canonical(vector<vector<bool>> mat) {
    string mat_repr = "";
    for (int r = 0; r < mat.size(); r++)
        for (int c = 0; c < mat[0].size(); c++)
            mat_repr += (mat[r][c]) ? "1" : "0";
    return mat_repr;
}

string get_canonical(sii component) {
    if (component.size() == 1) return "1";
    if (component.size() == 2) return "11";

    vector<vector<bool>> matrix;
    for (int r = left_top_corner.second; r <= right_bottom_corner.second; r++) {
        vector<bool> row;
        for (int c = left_top_corner.first; c <= right_top_corner.first; c++) {
            ii coord(c, r);
            row.push_back(component.find(coord) != component.end());
        }
        matrix.push_back(row);
    }
    return get_matrix_canonical(matrix);
}

bool components_equal(set<string> components_b1, set<string> components_b2) {
    for (string component_b1: components_b1) {
        if (components_b2.find(component_b1) != components_b2.end()) {
            components_b2.erase(component_b1);
        } else return false;
    }
    return components_b2.empty() ? true : false;
}

bool equivalent(sii items_b1, sii items_b2, int max_width, int max_height) {
    // Intialization
    for (int c = 0; c < 1000; c++) for (int r = 0; r < 1000; r++) {
        if (items_b1.find(ii(r, c)) != items_b1.end()) {
            b1[c][r] = true;
        } else b1[c][r] = false;
        if (items_b2.find(ii(r, c)) != items_b2.end()) {
            b2[c][r] = true;
        } else b2[c][r] = false;
    }
    visited_b1.clear(); visited_b2.clear();

    for (ii b1_item: items_b1) {
        sii connected_component;
        reset_bounds();
        dfs(b1_item, &visited_b1, &connected_component, max_width, max_height, items_b1);
        if (connected_component.empty()) continue;
        string mat_can = get_canonical(connected_component);
    }

    for (ii b2_item: items_b2) {
        sii connected_component;
        reset_bounds();
        dfs(b2_item, &visited_b2, &connected_component, max_width, max_height, items_b2);
        if (connected_component.empty()) continue;
        string mat_can = get_canonical(connected_component);
    }

    // For each board:
        // Reset bounds for each component
        // Get connected components by DFS for each item, keep visited items
        // Get canonical representation of the connected component
    // Compare canonical representations
}

int main()
{
    // improve perf
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tests, w, h, pieces, x, y;
    cin >> tests;
    while (tests > 0) {
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

        if (equivalent(items_b1, items_b2, w-1, h-1)) {
            cout << "YES\n";
        } else cout << "NO\n";

        tests--;
    }
}