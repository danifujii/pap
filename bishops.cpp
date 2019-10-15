#include <iostream>
#include <map>
#include <limits>

#define ull unsigned long long

using namespace std;

ull UNDEF = -1;

int add_item(int items, int i) {
    return items | (1 << i);
}

int remove_item(int items, int i) {
    return items & ~(1 << i);
}

bool is_available(int items, int i) {
    return (items & (1 << i)) > 0;
}

inline ull get_bishops_mem(int col, int k, int av_rows, map<int, ull> mem[][1000]) {
    auto m = mem[col][k];
    return m.find(av_rows) == m.end() ? UNDEF : m[av_rows];
}

ull bishops(int col, int k, int n, int av_rows, map<int, ull> mem[][1000]) {
    /*
     * In a single column, I should try inserting no element, one element and continue on with that
     * and then try with two (this can be continued with the "for available rows" of the first element).
     * I should check as many bits in av_rows as the number of col (i.e. if col == 0, then go to just that bit).
     * If the row is available, set it and use that as state. Setting is a matter of 1 << row & av_rows
     * Then, unsetting would be av_rows & ~(1 << row)
     * Remember middle column, where col == n, doesnt accept two in the same row
     */
    if (k <= 0) return (k == 0) ? 1 : 0;
    if (col > n) return 0;

    auto res = 0;
    auto zero_val = get_bishops_mem(col, k, av_rows, mem);
    if (zero_val == UNDEF)
        mem[col][k][av_rows] = bishops(col + 2, k, n, av_rows, mem); // 0 in this column
    res += mem[col][k][av_rows];

    for (int r = 0; r <= col; r++) {
        if (!is_available(av_rows, r)) continue;

        av_rows = remove_item(av_rows, r);
        auto one_val = get_bishops_mem(col, k - 1, av_rows, mem);
        if (one_val == UNDEF)
            mem[col][k-1][av_rows] = bishops(col + 2, k - 1, n, av_rows, mem);
        res += mem[col][k-1][av_rows] * (col < n ? 2 : 1);  // just one item in this column. Multiplied by 2 because you have to count for the symmetric case;

        if (col < n) {  // diagonal can't allow two
            for (int r2 = 0; r2 <= col; r2++) {  // add bishop in the other half of the board
                if (!is_available(av_rows, r2)) continue;
                av_rows = remove_item(av_rows, r2);
                auto two_val = get_bishops_mem(col, k - 2, av_rows, mem);
                if (two_val == UNDEF)
                    mem[col][k-2][av_rows] = bishops(col + 2, k - 2, n, av_rows, mem);  // just one item in this column
                res += mem[col][k-2][av_rows];
                av_rows = add_item(av_rows, r2);
            }
        }

        av_rows = add_item(av_rows, r);
    }
    return res;
}

ull bishops(int k, int n) {
    if (k == 0) return 1;

    map<int, ull> mem[n + 2][1000];  // current col, available bishops, available rows -> amount of combinations
    auto res = 0;
    for (int i = 0; i <= k; i++) {
        auto even = bishops(0, k - i, n - 1, numeric_limits<int>::max(), mem);
        auto uneven = bishops(1, i, n - 1, numeric_limits<int>::max(), mem);
        res += even * uneven;
    }
    return res;
}

int main()
{
    ios::sync_with_stdio(0); cin.tie(0);

    int n, k;
    while (true) {
        cin >> n >> k;
        if (n == 0) break;
        cout << bishops(k, n) << "\n";
    }
}
