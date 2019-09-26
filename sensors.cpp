#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <math.h>
#include <queue>

using namespace std;

typedef pair<int, int> ii;
typedef pair<long, ii> neigh;

/**
 * My idea is to implement Prim to get the MST from the house node.
 * Then, I'd like to go from each node of the tree that goes to/from house
 * and insert a receiver there until I run out of them. The way I go should be
 * from heaviest node to the lightest.
 * WAIT: That doesn't work, because we are allowing N nodes to go to the
 * house, more than we have receivers. So, the receivers should set up a
 * upper bound of nodes that can go to the house.
 * Let's do Prim, but limit the amount of branches from house to the number of
 * receivers.
 * Q : What happens when there are more receivers than branches to the house
 * that make sense to add to the MST?
 **/

long prim(ii root, map<ii, vector<neigh>> & graph) {
    map<ii, ii> parents;
    priority_queue<neigh, vector<neigh>, greater<neigh>> q;
    long cost = 0;

    q.push(neigh(0, root));
    while (!q.empty()) {
        neigh n = q.top(); q.pop();

        if (parents.find(n.second) != parents.end()) continue;
        cost += n.first;
        parents[n.second] = ii(0, 0);  // later see how to update this to save parents
        for (neigh ni: graph[n.second]) {
            if (parents.find(ni.second) != parents.end()) continue;
            q.push(ni);
        }
    }
    return cost;
}

long get_minimum_distance(map<ii, vector<neigh>> & graph, int receivers) {
    cout << prim(ii(50000, 50000), graph) << endl;
}

long get_distance(ii coord1, ii coord2) {
    return ceil(sqrt(pow(coord1.first-coord2.first, 2) + pow(coord1.first-coord2.first, 2)));
}

void add_to_graph(map<ii, vector<neigh>> & graph, ii new_coord) {
    vector<neigh> new_coord_neighs;

    // Look at the graph and calculate distances for all existing nodes
    // and add as neighbour
    for (pair<ii, vector<neigh>> coords: graph) {
        long distance = get_distance(new_coord, coords.first);
        coords.second.push_back(neigh(distance, new_coord));
        new_coord_neighs.push_back(neigh(distance, coords.first));
    }

    graph[new_coord] = new_coord_neighs;
}

int main() {
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
        ii house(50000, 50000);
        add_to_graph(graph, house);
        cout << get_minimum_distance(graph, receivers) << "\n";
    }
}
