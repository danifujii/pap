#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

typedef set<int> si;
typedef vector<int> vi;

si visited;
vi component;
si cycles;
vector<int> parents;

void dfs(int root, int parent, vector<vector<int>> & graph) {
    if (visited.find(root) != visited.end()) {
        if (find(component.begin(), component.end(), root) != component.end()) // cycle
            for (int i = component.size() - 1; i >= 0; --i) {
                cycles.insert(component[i]);
                if (component[i] == root) break;
            }
        return;
    }
    visited.insert(root);
    component.push_back(root);

    for (int n: graph[root]) {
        if (parents[n] == -1)
            parents[n] = parent;
        // if we reach one that already has a DFS tree parent and its part of a cycle and the parent as well,
        // then we should join the current DFS tree being built with the root of the DFS tree build for n
        else if (cycles.find(n) != cycles.end() && cycles.find(parents[n]) != cycles.end()) {
            parents[parents[n]] = parent;
        }
        dfs(n, parent, graph);
    }
    component.pop_back();
}

void solve(vector<vector<int>> & graph, int test) {
    visited.clear(); parents.clear(); cycles.clear();
    for (int i = 0; i < graph.size(); i++) { parents.push_back(-1); }

    for (int i = 0; i < graph.size(); i++) {
        component.clear();
        if (visited.find(i) != visited.end()) continue;
        dfs(i, i, graph);
    }

    if (test == 137) {
        auto p = parents;
        auto c = cycles;
        int i = 1;
    }

    long amount = 0;
    for (int i = 0; i < parents.size(); i++) {
        if (parents[i] == -1 || parents[i] == i) amount += 1;
    }
    cout << amount << "\n";
}

int main() {
    // improve perf
    ios::sync_with_stdio(0); cin.tie(0);

    int tests;
    cin >> tests;

    int t = 1;
    while (tests > 0) {
        int dominoes, lines, domino, neigh;
        cin >> dominoes >> lines;
        vector<vector<int>> graph(dominoes);
        for (int i = 0; i < lines; i++) {
            cin >> domino >> neigh;
            graph[domino-1].push_back(neigh-1);
        }
        solve(graph, t);
        --tests;
        t++;
    }
}