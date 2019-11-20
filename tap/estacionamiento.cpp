#include <iostream>
#include <vector>
#include <iomanip>
#include <map>
#include <unordered_map>

using namespace std;

typedef pair<int, int> pi;

// From [current_pos][other_end][blocks] -> probability
unordered_map<int, unordered_map<int, unordered_map<int, double>>> mem;
int INF = 100000;

double get_blocks(const vector<double> & probs, int pos, int blocks, pair<int, int> visited) {
    visited.first = min(visited.first, pos); visited.second = max(visited.second, pos);
    int other_end = visited.first == pos ? visited.second : visited.first;

    if (mem.find(pos) == mem.end() || mem[pos].find(other_end) == mem[pos].end() || mem[pos][other_end].find(blocks) == mem[pos][other_end].end()) {
        double p_here = blocks * probs[pos];
        double p_best = INF;
        if (visited.first > 0) { // can go left and we havent been all the way left
            int walk_left_blocks = blocks + (pos - (visited.first - 1));
            p_best = min(p_best, get_blocks(probs, visited.first-1, walk_left_blocks, visited));
        }
        if (visited.second < probs.size() - 1) { // can go right
            int walk_right_blocks = blocks + (visited.second+1 - pos);
            p_best = min(p_best, get_blocks(probs, visited.second+1, walk_right_blocks, visited));
        }
        mem[pos][other_end][blocks] = p_here + (p_best != INF ? p_best : 0);
    }
    return mem[pos][other_end][blocks];
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);

    int m; cin >> m;
    int mid = (2*m+1) / 2;

    double p;
    vector<double> probs(2*m + 1);
    for (int i = 0; i < m; ++i) {
        cin >> p;
        probs[i] = p;
    }
    probs[mid] = 0;  // starting point
    for (int i = mid + 1; i < 2*m + 1; ++i) {
        cin >> p;
        probs[i] = p;
    }
    double result = get_blocks(probs, mid, 0, pi(mid, mid));

    cout << fixed; cout << setprecision(6);
    cout << result << "\n";
}
