#include <iostream>
#include <vector>
#include <map>
#include <bitset>

using namespace std;

typedef vector<int> vi;
typedef vector<char> vc;

map<int, vc> mem;
vi colors;

vc dfs(const vector<vi> & graph, const vector<bool> & has_roq, int d, vc & so_far) {
    if (d-1 == 0) return so_far;
    vc best(d, '0');

}

void solve(const vector<vi> & graph, int d, const vector<bool> & has_roq) {
    colors.resize(graph.size(), 0);

    for (int g = 0; g < graph.size(); g++) {
        if (colors[g] == 0) {
            dfs(graph, has_roq, d);
            colors[g] = 1;
        }
    }

    // Build solution
    vc best(d, '0');
    for (int g = 0; g < graph.size(); g++) {
        for (int n: graph[g]) {
            vc n_best = mem[n];
            if (n_best.size() == d-1) {
                vc new_nbest = vc(n_best.begin(), n_best.end()-1);
                new_nbest.insert(n_best.begin(), has_roq[g] ? '1' : '0');
                if (best < new_nbest) best = new_nbest;
            }
        }
    }
}

int main()
{
    // improve perf
    ios::sync_with_stdio(0); cin.tie(0);

    int n, d, roq, e;
    cin >> n >> d;

    vector<bool> has_roq(n);
    for (int i = 0; i < n; i++) {
        cin >> roq;
        has_roq[i] = roq > 0;
    }

    vector<vi> graph(n);
    for (int i = 0; i < n; i++) {
        cin >> e;
        if (e != -1) graph[e].push_back(i);
    }
    solve(graph, d, has_roq);
}