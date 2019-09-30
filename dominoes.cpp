#include <iostream>
#include <set>
#include <vector>

using namespace std;

typedef set<int> si;

si visited;
si component;
si cycles;
vector<int> parents;

void dfs(int root, int parent, vector<vector<int>> & graph) {
    if (visited.find(root) != visited.end()) {
        auto c = component;
        if (component.find(root) != component.end()) // cycle
            for (int i: component) cycles.insert(i);
        return;
    }
    visited.insert(root);
    component.insert(root);

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
    component.erase(root);
}

void solve(vector<vector<int>> & graph) {
    visited.clear(); parents.clear(); cycles.clear();
    for (int i = 0; i < graph.size(); i++) { parents.push_back(-1); }

    for (int i = 0; i < graph.size(); i++) {
        if (visited.find(i) != visited.end()) continue;
        component.clear();
        dfs(i, i, graph);
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

    while (tests > 0) {
        int dominoes, lines, domino, neigh;
        cin >> dominoes >> lines;
        vector<vector<int>> graph(dominoes);
        for (int i = 0; i < lines; i++) {
            cin >> domino >> neigh;
            graph[domino-1].push_back(neigh-1);
        }
        solve(graph);
        --tests;
    }
}