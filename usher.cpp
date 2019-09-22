#include <iostream>
#include <vector>

using namespace std;

typedef vector<long> vl;

long INF = 1000001;

vl get_empty_adjs(int size, int node) {
    vl adjs(size);
    for (int i = 0; i < adjs.size(); i++) adjs[i] = INF;
    adjs[node] = 0;
    return adjs;
}

void floyd(const vector<vl> & graph, vector<vl> & dist) {
    for (int k = 0; k < graph.size(); k++)
        for (int i = 0; i < graph.size(); i++)
            for (int j = 0; j < graph.size(); j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
}

void solve(const vector<vl> & graph, int box) {
    // Init distances matrix for Floyd Warshall
    vector<vl> dist(graph.size());
    for (int i = 0; i < graph.size(); i++) {
        vl adjs(graph.size());
        for (int j = 0; j < graph.size(); j++)
            adjs[j] = graph[i][j];
        dist[i] = adjs;
    }

    floyd(graph, dist); // this updates `dist`

    // Get solution: get the one with the smallest path from a neighbour i of usher to the usher
    long min_path = INF;
    for (int i = 1; i < graph.size(); i++)
        if (graph[0][i] != INF) min_path = min(min_path, dist[i][0]);

    int usher_takes = 0;
    int sum = min_path;
    while (sum < box) {
        usher_takes += 1;
        sum += min_path - 1;
    }
    cout << usher_takes << "\n";
}

int main()
{
    // improve perf
    ios::sync_with_stdio(0); cin.tie(0);

    long datasets, pers, box, neigh_length, neigh, cost;
    cin >> datasets;
    for (int d = 0; d < datasets; d++) {
        cin >> box >> pers;
        vector<vl> graph(pers+1);

        cin >> neigh_length;
        vl usher_adj = get_empty_adjs(pers+1, 0);
        for (int i = 0; i < neigh_length; i++) {
            cin >> neigh; usher_adj[neigh] = 0;
        }
        graph[0] = usher_adj;

        for (int p = 1; p <= pers; p++) {
            vl adjs = get_empty_adjs(pers+1, p);
            cin >> neigh_length;
            for (int j = 0; j < neigh_length; j++) {
                cin >> cost >> neigh;
                adjs[neigh] = min(adjs[neigh], cost);
            }
            graph[p] = adjs;
        }
        solve(graph, box);
    }
}