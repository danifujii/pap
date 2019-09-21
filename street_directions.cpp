#include <iostream>
#include <vector>
#include <set>

using namespace std;

typedef vector<int> vi;
typedef pair<int, int> ii;

vector<bool> visited;
set<ii> bridges;
vi parent, dfs_low, dfs_depth;
int dfs_counter;

void dfs(int node, const vector<vi> & graph) {
    visited[node] = true;
    dfs_low[node] = dfs_depth[node] = dfs_counter++;

    for (int neighbour: graph[node]) {
        if (neighbour == parent[node]) continue;

        if (visited[neighbour]) {
            dfs_low[node] = min(dfs_low[node], dfs_depth[neighbour]);
        } else {
            parent[neighbour] = node;
            dfs(neighbour, graph);
            dfs_low[node] = min(dfs_low[node], dfs_low[neighbour]);
            if (dfs_low[neighbour] > dfs_depth[node]) {
                bridges.insert(ii(node, neighbour));
                bridges.insert(ii(neighbour, node));
            }
        }
    }
}

void dfs_print(int node, vector<vi> & graph) {
    visited[node] = true;
    for (int neighbour: graph[node]) {
        if (neighbour == -1) continue;

        cout << node+1 << " " << neighbour+1 << "\n";

        // remove the edge from neighbour to node if its not bridge
        if (bridges.find(ii(node, neighbour)) == bridges.end()) {
            vi * n_edges = &graph[neighbour];
            int node_index = -1;
            for (int i = 0; i < n_edges->size(); i++) if (n_edges->at(i) == node) node_index = i;
            if (node_index != -1) n_edges->at(node_index) = -1;
        }

        if (!visited[neighbour]) {
            dfs_print(neighbour, graph);
        }
    }
}

void solve(vector<vi> & graph) {
    // Init
    for (int i = 0; i < parent.size(); i++){ visited[i] = false; dfs_low[i] = parent[i] = dfs_depth[i] = -1; }
    dfs_counter = 0;

    for (int i = 0; i < graph.size(); i++)
        if (!visited[i])
            dfs(i, graph);

    for (int i = 0; i < parent.size(); i++) visited[i] = false;

    for (int i = 0; i < graph.size(); i++)
        if (!visited[i])
            dfs_print(i, graph);
}

int main()
{
    // improve perf
    ios::sync_with_stdio(0);
    cin.tie(0);

    int streets, inters = -1;
    int cas = 0;
    while (true) {
        cas++;
        cin >> inters >> streets;
        if (inters == 0) break;

        // Cleanup and init
        parent.clear(); dfs_low.clear(); dfs_depth.clear(); visited.clear(); bridges.clear();
        parent.resize(inters); dfs_low.resize(inters); dfs_depth.resize(inters); visited.resize(inters);

        vector<vi> graph(inters);
        int i1, i2;
        for (int i = 0; i < streets; i++) {
            cin >> i1 >> i2;
            --i1; --i2;  // normalize indices
            graph[i1].push_back(i2); graph[i2].push_back(i1);
        }
        cout << cas << "\n";
        cout << "\n";
        solve(graph);
        cout << "#\n";
    }
}
