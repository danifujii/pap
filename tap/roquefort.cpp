#include <iostream>
#include <vector>
#include <map>
#include <bitset>

using namespace std;

typedef vector<int> vi;

map<int, string> mem;
vi colors;

string dfs(const vector<vi> & graph, int root, const vector<bool> & has_roq, int d) {
    if (colors[root] == 2) return mem[root];
    if (d == 0 && colors[root] == 1) return has_roq[root] ? "1" : "0";
    colors[root] = 1;

    string best = "";
    for (int n: graph[root]) {
        string n_str = dfs(graph, n, has_roq, d-1);
        mem[n] = n_str;
        if (n_str > best) best = n_str;
    }

    colors[root] = 2;
    best.insert(best.begin(), has_roq[root] ? '1' : '0');
    return best;
}

void solve(const vector<vi> & graph, int d, const vector<bool> & has_roq) {
    colors.resize(graph.size(), 0);

    for (int g = 0; g < graph.size(); g++) {
        if (colors[g] == 0) {
            mem[g] = dfs(graph, g, has_roq, d-1);
            colors[g] = 2;
        }
    }

    // TODO probably best to build all possible of len D, write in mem and then try to do this
    // Build solution
    string best = "";
    for (int g = 0; g < graph.size(); g++) {
        string mem_g = mem[g].substr(0, d);
        if (mem[g].size() >= d) { best = max(best, mem[g].substr(0, d)); continue; }

        string n_best = "";
        for (int n: graph[g])
            if (mem[n].size() >= d-1) n_best = max(n_best, mem[n].substr(0, d-1));
        if (n_best.size() == d-1) {
            n_best.insert(n_best.begin(), has_roq[g] ? '1' : '0');
            best = max(best, n_best);
        }
    }
    for (char c: best) {
        cout << c << " ";
    }
    cout << "\n";
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
        if (e != -1) graph[e-1].push_back(i);
    }
    solve(graph, d, has_roq);
}