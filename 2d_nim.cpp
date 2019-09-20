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

void dfs(ii root, sii* visited, sii* connected_component, int max_width, int max_height, sii & nodes) {
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

vector<vb>* flip_horizontal(vector<vb>* mat) {
    int row_length = mat->at(0).size();
    int middle = row_length%2 == 0 ? row_length/2 : row_length/2+1;

    for (int r = 0; r < mat->size(); r++) {
        for (int c = 0; c < middle; c++) {
            bool aux = mat->at(r)[row_length-c-1];
            mat->at(r)[row_length-c-1] = mat->at(r)[c];
            mat->at(r)[c] = aux;
        }
    }
    return mat;
}

vector<vb>* flip_vertical(vector<vb>* mat) {
    int middle = mat->size()%2 == 0 ? mat->size()/2 : mat->size()/2+1;

    for (int r = 0; r < middle; r++)
        for (int c = 0; c < mat->at(0).size(); c++) {
            bool aux = mat->at(mat->size()-r-1)[c];
            mat->at(mat->size()-r-1)[c] = mat->at(r)[c];
            mat->at(r)[c] = aux;
        }
    return mat;
}

vector<vb> flip_90(vector<vb> & mat) {
    vector<vb> flipped(mat[0].size());
    for (int i = 0; i < mat[0].size(); i++) flipped[i] = vb(mat.size());
    for (int r = 0; r < mat.size(); r++)
        for (int c = 0; c < mat[0].size(); c++)
            flipped[c][mat.size()-r-1] = mat[r][c];
    return flipped;
}

vector<ii> get_matrix_canonical(vector<vb>* mat) {
    vector<ii> rv;
    for (int r = 0; r < mat->size(); r++) {
        for (int c = 0; c < mat->at(0).size(); c++)
            if (mat->at(r)[c]) {
                rv.push_back(ii(r, c));
            }
    }
    sort(rv.begin(), rv.end());
    return rv;
}

vector<ii> get_canonical(sii & component) {
    vector<vb> matrix;
    for (int r = left_top_corner.second; r <= right_bottom_corner.second; r++) {
        vb row;
        for (int c = left_top_corner.first; c <= right_bottom_corner.first; c++) {
            ii coord(c, r);
            row.push_back(component.find(coord) != component.end());
        }
        matrix.push_back(row);
    }

    vector<ii> min_canonical = get_matrix_canonical(&matrix);

    // To avoid complicating myself later on, building rotated after matrix has been changed
    vector<vb> rotated = flip_90(matrix);

    // Keep dimensions
    min_canonical = min(min_canonical, get_matrix_canonical(flip_vertical(&matrix)));
    min_canonical = min(min_canonical, get_matrix_canonical(flip_horizontal(&matrix)));  // 180 degrees
    min_canonical = min(min_canonical, get_matrix_canonical(flip_vertical(&matrix)));  // flip horizontal of og matrix

    // Flip comuns size by rows size
    min_canonical = min(min_canonical, get_matrix_canonical(&rotated));
    min_canonical = min(min_canonical, get_matrix_canonical(flip_horizontal(&rotated)));
    min_canonical = min(min_canonical, get_matrix_canonical(flip_vertical(&rotated)));
    min_canonical = min(min_canonical, get_matrix_canonical(flip_horizontal(&rotated)));  // 270 degrees
    return min_canonical;
}

bool equivalent(sii & items_b1, sii & items_b2, int max_width, int max_height) {
    // Intialization
    visited_b1.clear(); visited_b2.clear();
    set<vector<ii>> canonical_comps_b1, canonical_comps_b2;
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

    return canonical_comps_b1 == canonical_comps_b2;
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

        if (equivalent(items_b1, items_b2, w, h)) {
            cout << "YES\n";
        } else cout << "NO\n";

        tests--;
    }
}