#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <math.h>
#include <queue>

using namespace std;

typedef long long ll;
typedef pair<int, int> ii;
typedef pair<ll, ii> neigh;
typedef pair<neigh, ii> mst_node; // neigh is node with weight. ii is parent

int land_size = 100000;
ii house = ii(land_size/2, land_size/2);

ii find(map<ii, ii> & parent, ii node) {
    if (node != parent[node])
        parent[node] = find(parent, parent[node]);
    return parent[node];
}

void merge(map<ii, ii> & parent, ii node1, ii node2) {
    ii p1 = find(parent, node1);
    ii p2 = find(parent, node2);
    parent[p2] = p1;
}

ll kruskal(map<ii, vector<neigh>> & graph, int receivers) {
    ll max_weight = 0;

    // Make set
    map<ii, ii> parent;
    for (pair<ii, vector<neigh>> coord: graph) { parent[coord.first] = coord.first; }

    // G.E ordered by weight
    int components = graph.size();
    priority_queue<mst_node, vector<mst_node>, greater<mst_node>> q;
    for (pair<ii, vector<neigh>> coord: graph) for (neigh n: coord.second) q.push(mst_node(n, coord.first));

    // Kruskal itself
    while (!q.empty()) {
        mst_node n = q.top(); q.pop();
        ll weight = n.first.first;
        ii u = n.first.second; ii v = n.second;
        if (find(parent, u) != find(parent, v)) {
            max_weight = max(max_weight, weight);
            merge(parent, u, v);
            --components;
        }

        if (components == receivers) break;
    }

    return q.top().first.first;
}

ll get_minimum_distance(map<ii, vector<neigh>> & graph, int receivers) {
    return kruskal(graph, receivers);
}

ll get_distance(ii coord1, ii coord2) {
    return ceil(sqrt(pow(coord1.first-coord2.first, 2) + pow(coord1.second-coord2.second, 2)));
}

void add_to_graph(map<ii, vector<neigh>> & graph, ii new_coord) {
    vector<neigh> new_coord_neighs;

    // Look at the graph and calculate distances for all existing nodes
    // and add as neighbour
    for (pair<ii, vector<neigh>> coords: graph) {
        ii neigh_coords = coords.first;
        ll distance = get_distance(new_coord, neigh_coords);
        graph[neigh_coords].push_back(neigh(distance, new_coord));
        new_coord_neighs.push_back(neigh(distance, neigh_coords));
    }

    graph[new_coord] = new_coord_neighs;
}

int main() {
    // improve perf
    ios::sync_with_stdio(0); cin.tie(0);

    int tests, receivers, x, y;
    cin >> tests;

    for (int t = 0; t < tests; t++) {
        cin >> receivers;

        map<ii, vector<neigh>> graph;
        while (true) {
            cin >> x;
            if (x == -1) break;
            else cin >> y;

            add_to_graph(graph, ii(x, y));
        }
        cout << get_minimum_distance(graph, receivers) << "\n";
    }
}
