#include <iostream>
#include <vector>

using namespace std;

typedef vector<int> vi;

vi get_empty_adjs(int size) {
    vi adjs(size);
    for (int i = 0; i < adjs.size(); i++) adjs[i] = 1000001;
    return adjs;
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
    }
}