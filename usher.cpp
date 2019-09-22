#include <iostream>
#include <vector>

using namespace std;

typedef vector<int> vi;

int INF = 1000001;

vi get_empty_adjs(int size) {
    vi adjs(size);
    for (int i = 0; i < adjs.size(); i++) adjs[i] = INF;
    return adjs;
}

void floyd(const vector<vi> & graph, vector<vi> & dist) {
    for (int k = 0; k < graph.size(); k++)
        for (int i = 0; i < graph.size(); i++)
            for (int j = 0; j < graph.size(); j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
}

void solve(const vector<vi> & graph, int box) {
    // Init distances matrix for Floyd Warshall
    vector<vi> dist(graph.size());
    for (int i = 0; i < graph.size(); i++) {
        vi adjs(graph.size());
        for (int j = 0; j < graph.size(); j++)
            adjs[j] = graph[i][j];
    }

    floyd(graph, dist); // this updates `dist`

    // Get solution
    int min_path = INF;
    for (int i = 0; i < graph.size(); i++)
        if (graph[0][i] != INF) min_path = min(min_path, graph[0][i]);
    cout << box / min_path << "\n";
}

int main()
{
    // improve perf
    ios::sync_with_stdio(0); cin.tie(0);

    int datasets, pers, box, neigh_length, neigh, cost;
    cin >> datasets;
    for (int d = 0; d < datasets; d++) {
        cin >> box >> pers;
        vector<vi> graph(pers+1);

        cin >> neigh_length;
        vi usher_adj = get_empty_adjs(neigh_length);
        for (int i = 0; i < neigh_length, neigh; i++) {
            cin >> neigh; usher_adj[neigh] = 0;
        }
        graph[0] = usher_adj;

        for (int p = 1; p <= pers; p++) {
            vi adjs = get_empty_adjs(pers);
            cin >> neigh_length;
            for (int j = 0; j < neigh_length; j++) {
                cin >> cost >> neigh;
                if (neigh == 0) --cost;
                adjs[neigh] = cost;
            }
            graph[p] = adjs;
        }
        solve(graph, box);
    }
}