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
    int middle = row_length/2;

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
    int middle = mat->size()/2;
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

vector<vb> get_component_matrix(const sii & component) {
    vector<vb> matrix;
    for (int r = left_top_corner.second; r <= right_bottom_corner.second; r++) {
        vb row;
        for (int c = left_top_corner.first; c <= right_bottom_corner.first; c++) {
            ii coord(c, r);
            row.push_back(component.find(coord) != component.end());
        }
        matrix.push_back(row);
    }
    return matrix;
}

bool equal(const vector<vb> & mat1, vector<vb> & mat2) {
    // To avoid complicating myself later on, building rotated after matrix has been changed
    vector<vb> rotated = flip_90(mat2);

    if (mat1 == mat2) return true;
    flip_vertical(&mat2); if (mat1 == mat2) return true;
    flip_horizontal(&mat2); if (mat1 == mat2) return true;  // 180 degrees
    flip_vertical(&mat2); if (mat1 == mat2) return true;  // flip horizontal of og matrix
    flip_horizontal(&mat2);  // return mat2 to original shape

    // Flip comuns size by rows size
    if (mat1 == rotated) return true;
    flip_horizontal(&rotated); if (mat1 == rotated) return true;
    flip_vertical(&rotated); if (mat1 == rotated) return true;
    flip_horizontal(&rotated); if (mat1 == rotated) return true;  // 270 degrees
    return false;
}

bool equivalent(sii & items_b1, sii & items_b2, int max_width, int max_height) {
    // Intialization
    visited_b1.clear(); visited_b2.clear();
    vector<vector<vb>> comps_b1, comps_b2;
    sii connected_component;

    for (ii b1_item: items_b1) {
        connected_component.clear();
        reset_bounds();
        if (visited_b1.find(b1_item) != visited_b1.end()) continue;
        dfs(b1_item, &visited_b1, &connected_component, max_width, max_height, items_b1);
        comps_b1.push_back(get_component_matrix(connected_component));
    }

    for (ii b2_item: items_b2) {
        connected_component.clear();
        reset_bounds();
        if (visited_b2.find(b2_item) != visited_b2.end()) continue;
        dfs(b2_item, &visited_b2, &connected_component, max_width, max_height, items_b2);
        comps_b2.push_back(get_component_matrix(connected_component));
    }

    if (comps_b1.size() != comps_b2.size()) return false;

    for (vector<vb> component_b1: comps_b1) {
        for (auto it = comps_b2.begin(); it != comps_b2.end(); it++) {
            auto component_b2 = *it;
            if ((component_b1.size() == component_b2.size() && component_b1[0].size() == component_b2[0].size())
                || (component_b1.size() == component_b2[0].size() && component_b1[0].size() == component_b2.size())) {
                    if (equal(component_b1, component_b2)) {
                        comps_b2.erase(it);
                        break;
                    }
                }
        }
    }

    return comps_b2.empty();
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