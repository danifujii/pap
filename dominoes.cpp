#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

typedef set<int> si;
typedef vector<int> vi;

vi component;

void dfs(int root, vector<vector<int>> & graph, vector<bool> & visited) {
    if (visited[root]) return;
    visited[root] = true;

    for (int n: graph[root]) dfs(n, graph, visited);
    component.push_back(root);
}

void solve(vector<vector<int>> & graph) {
    vector<bool> visited(graph.size(), false);
    component.clear();

    for (int i = 0; i < graph.size(); ++i) {
        if (!visited[i]) dfs(i, graph, visited);
    }

    long amount = 0;
    visited.assign(graph.size(), false);
    for (int i = component.size() - 1; i >= 0; --i) {
        int v = component[i];
        if (!visited[v]) {
            dfs(v, graph, visited);
            ++amount;
        }
    }
    cout << amount << "\n";
}

int main() {
    // improve perf
    ios::sync_with_stdio(0); cin.tie(0);

    int tests, dominoes, lines, domino, neigh;
    cin >> tests;

    while (tests > 0) {
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