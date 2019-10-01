#include <iostream>
#include <vector>
#include <map>
#include <queue>

using namespace std;

typedef pair<int, int> ii;
typedef pair<long, int> node;

long INF = 1000001;

void solve(map<int, map<int, long>> & graph, int box) {
    // Init
    vector<long> dist(graph.size()+1);
    priority_queue<node, vector<node>, greater<node>> q;

    for (int i = 0; i < dist.size(); i++) {
        dist[i] = INF;
        q.push(node(INF, i));
    }
    for (pair<int, long> e: graph[0]) {
        dist[e.first] = e.second;
        q.push(node(e.second, e.first));
    }

    while (!q.empty()) {
        int u = q.top().second; q.pop();
        for (pair<int, long> e: graph[u]) {
            long new_dist = dist[u] + e.second;
            long prev_dist = dist[e.first];
            if (new_dist < prev_dist) {
                dist[e.first] = new_dist;
                q.push(node(dist[e.first], e.first));
            }
        }
    }

    int usher_takes = 0;
    int sum = dist[0];
    while (sum < box) {
        usher_takes += 1;
        sum += dist[0] - 1;
    }
    std::cout << usher_takes << "\n";
}

int main()
{
    // improve perf
    ios::sync_with_stdio(0); cin.tie(0);

    long datasets, pers, box, neigh_length, neigh, cost;
    cin >> datasets;
    for (int d = 0; d < datasets; d++) {
        cin >> box >> pers;
        map<int, map<int, long>> graph;

        cin >> neigh_length;
        for (int i = 0; i < neigh_length; i++) {
            cin >> neigh;
            graph[0][neigh] = 0;
        }

        for (int p = 1; p <= pers; p++) {
            cin >> neigh_length;
            for (int j = 0; j < neigh_length; j++) {
                cin >> cost >> neigh;
                if (graph.find(p) != graph.end() && graph[p].find(neigh) != graph[p].end()) {
                    graph[p][neigh] = min(graph[p][neigh], cost);
                } else {
                    graph[p][neigh] = cost;
                }
            }
        }
        solve(graph, box);
    }
}