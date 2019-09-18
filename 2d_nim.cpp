#include <iostream>
#include <algorithm>
#include <set>
#include <vector>

using namespace std;

typedef pair<int, int> ii;
typedef set<ii> sii;
typedef vector<bool> vb;

ii left_top_corner, right_bottom_corner;
sii visited_b1, visited_b2;

sii neighbors(ii coord, int max_width, int max_height) {
    sii rv;
    if (coord.first > 0) rv.insert(ii(coord.first-1, coord.second));
    if (coord.first < max_width) rv.insert(ii(coord.first+1, coord.second));
    if (coord.second > 0) rv.insert(ii(coord.first, coord.second-1));
    if (coord.second < max_height) rv.insert(ii(coord.first, coord.second+1));
    return rv;
}

void update_bounds(ii coord) {
    if (coord.first < left_top_corner.first) left_top_corner.first = coord.first;
    if (coord.second < left_top_corner.second) left_top_corner.second = coord.second;
    if (coord.first > right_bottom_corner.first) right_bottom_corner.first = coord.first;
    if (coord.second > right_bottom_corner.second) right_bottom_corner.second = coord.second;
}

void reset_bounds() {
    left_top_corner.first = 1005; right_bottom_corner.first = -1;
    left_top_corner.second = 1005; right_bottom_corner.second = -1;
}

void dfs(ii root, sii* visited, sii* connected_component, int max_width, int max_height, sii nodes) {
    if (visited->find(root) != visited->end()) return;

    visited->insert(root);
    connected_component->insert(root);
    update_bounds(root);

    for (ii n: neighbors(root, max_width, max_height)) {
        if (nodes.find(n) != nodes.end()) {
            dfs(n, visited, connected_component, max_width, max_height, nodes);
        }
    }
}

vector<vb> flip_horizontal(vector<vb> mat) {
    vector<vb> flipped;
    int row_length = mat[0].size();
    for (int r = 0; r < mat.size(); r++) {
        vb flipped_row(row_length);
        for (int c = 0; c < row_length; c++) flipped_row[row_length-c-1] = mat[r][c];
        flipped.push_back(flipped_row);
    }
    return flipped;
}

vector<vb> flip_vertical(vector<vb> mat) {
    vector<vb> flipped(mat.size());
    for (int i = 0; i < mat.size(); i++) flipped[i] = vb(mat[0].size());
    for (int r = 0; r < mat.size(); r++)
        for (int c = 0; c < mat[0].size(); c++) flipped[mat.size()-r-1][c] = mat[r][c];
    return flipped;
}

vector<vb> flip_90(vector<vb> mat) {
    vector<vb> flipped(mat[0].size());
    for (int i = 0; i < mat[0].size(); i++) flipped[i] = vb(mat.size());
    for (int r = 0; r < mat.size(); r++)
        for (int c = 0; c < mat[0].size(); c++)
            flipped[c][mat.size()-r-1] = mat[r][c];
    return flipped;
}

vector<vb> flip_270(vector<vb> mat) {
    vector<vb> flipped(mat[0].size());
    for (int i = 0; i < mat[0].size(); i++) flipped[i] = vb(mat.size());
    for (int r = 0; r < mat.size(); r++)
        for (int c = 0; c < mat[0].size(); c++)
            flipped[mat[0].size()-c-1][r] = mat[r][c];
    return flipped;
}

string get_matrix_canonical(vector<vb> mat) {
    string mat_repr = "";
    for (int r = 0; r < mat.size(); r++)
        for (int c = 0; c < mat[0].size(); c++)
            mat_repr += (mat[r][c]) ? "1" : "0";
    return mat_repr;
}

string get_canonical(sii component) {
    vector<vb> matrix;
    for (int r = left_top_corner.second; r <= right_bottom_corner.second; r++) {
        vb row;
        for (int c = left_top_corner.first; c <= right_bottom_corner.first; c++) {
            ii coord(c, r);
            row.push_back(component.find(coord) != component.end());
        }
        matrix.push_back(row);
    }

    // Single row/column components are simply just a line of 1s in every variation
    if (matrix.size() == 1 || matrix[0].size() == 1)
        return get_matrix_canonical(matrix);

    string min_canonical = get_matrix_canonical(matrix);
    min_canonical = min(min_canonical, get_matrix_canonical(flip_horizontal(matrix)));
    min_canonical = min(min_canonical, get_matrix_canonical(flip_vertical(matrix)));
    min_canonical = min(min_canonical, get_matrix_canonical(flip_90(matrix)));
    min_canonical = min(min_canonical, get_matrix_canonical(flip_horizontal(flip_vertical(matrix))));
    min_canonical = min(min_canonical, get_matrix_canonical(flip_270(matrix)));
    min_canonical = min(min_canonical, get_matrix_canonical(flip_horizontal(flip_270(matrix))));
    min_canonical = min(min_canonical, get_matrix_canonical(flip_horizontal(flip_90(matrix))));
    return min_canonical;
}

bool components_equal(set<string> components_b1, set<string> components_b2) {
    for (string component_b1: components_b1) {
        if (components_b2.find(component_b1) != components_b2.end()) {
            components_b2.erase(component_b1);
        } else return false;
    }
    return components_b2.empty();
}

bool equivalent(sii items_b1, sii items_b2, int max_width, int max_height) {
    // Intialization
    visited_b1.clear(); visited_b2.clear();
    set<string> canonical_comps_b1, canonical_comps_b2;
    sii connected_component;

    for (ii b1_item: items_b1) {
        connected_component.clear();
        reset_bounds();
        if (visited_b1.find(b1_item) != visited_b1.end()) continue;
        dfs(b1_item, &visited_b1, &connected_component, max_width, max_height, items_b1);
        canonical_comps_b1.insert(get_canonical(connected_component));
    }

    for (ii b2_item: items_b2) {
        connected_component.clear();
        reset_bounds();
        if (visited_b2.find(b2_item) != visited_b2.end()) continue;
        dfs(b2_item, &visited_b2, &connected_component, max_width, max_height, items_b2);
        canonical_comps_b2.insert(get_canonical(connected_component));
    }

    if (canonical_comps_b1.size() != canonical_comps_b2.size()) return false;
    return components_equal(canonical_comps_b1, canonical_comps_b2);
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