#include <iostream>
#include <vector>
#include <limits>

using namespace std;

int INT_MAX = numeric_limits<int>::max();
int INT_MIN = numeric_limits<int>::min();
int UNDEF = -10;

vector<int> elems;
int M[10000][10000];


int knapsack(int n, long w) {
    if (w < 0) return INT_MIN;
    if (n < 0) return 0;
    if (M[n][w] == UNDEF)
        M[n][w] = max(knapsack(n-1, w), elems[n] + knapsack(n-1, w-elems[n]));
    return M[n][w];
}

void solution(int n, int w_p, int w_s, int optim, vector<string> *sol) {
    if ((w_p <= 0 && w_s <= 0) || n < 0) return;

    if (n == 0) {
        if (elems[n]==optim) sol->push_back("port\n");
        else if (w_s-elems[n] >= 0) sol->push_back("starboard\n");
        return;
    }

    if (w_p-elems[n] >= 0 && M[n-1][w_p-elems[n]] == optim-elems[n]) {
        sol->push_back("port\n");
        solution(n-1, w_p-elems[n], w_s, optim-elems[n], sol);
    } else if (M[n-1][w_p] == optim && w_s-elems[n] >= 0) {
        // If it isn't part of p, I should be able to fit it in s to continue
        sol->push_back("starboard\n");
        solution(n-1, w_p, w_s-elems[n], optim, sol);
    }
}

void create_solution(long size) {
    for (int i = 0; i < 10000; i++)
        for (int j = 0; j < 10000; j++)
            M[i][j] = UNDEF;

    vector<string> sol;
    auto optim = knapsack(elems.size()-1, size);
    solution(elems.size()-1, size, size, optim, &sol);

    cout << sol.size() << "\n";
    for (int i = 0; i < sol.size(); i++) cout << sol[i];
}

int main(int argc, char const *argv[])
{
    // improve perf
    ios::sync_with_stdio(0);
    cin.tie(0);

    long cases, size, car;
    cin >> cases;

    for (int c = 0; c < cases; c++) {
        cin >> size;
        size *= 100;

        elems.clear();
        while (true) {  // read cars
            cin >> car;
            if (car == 0) break;
            else elems.insert(elems.begin(), car);
        }
        create_solution(size);
        cout << "\n";
    }

    return 0;
}
