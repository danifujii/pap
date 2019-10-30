#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

typedef vector<int> vi;
typedef vector<bool> vb;

vi compute_pi(const string & w) {
    vi pi(w.size()+1, 0);
    int len;
    for (int i = 1; i < w.size(); ++i) {
        len = pi[i-1];
        while (len > 0 && w[len] != w[i])
            len = pi[len - 1];
        if (w[i] == w[len]) len++;
        pi[i] = len;
    }
    return pi;
}

// KMP modified to work with several patterns at the same time
vb kmp_search(const string &text, const vector<string> &patterns) {
    vector<vi> pis(patterns.size()); vi lens(patterns.size(), 0);
    vb found(patterns.size(), false), deleted(patterns.size(), false);

    for (int i = 0; i < patterns.size(); ++i) { pis[i] = compute_pi(patterns[i]); }

    for (int i = 0; i < text.size(); ++i) {
        for (int j = 0; j < patterns.size(); ++j) {
            int len = lens[j];
            while (len > 0 && patterns[j][len] != text[i])
                len = pis[j][len-1];
            lens[j] = len;
        }
        for (int j = 0; j < patterns.size(); ++j) {
            if (deleted[j]) continue;  // this pattern already appeared twice, not useful

            int len = lens[j];
            if (text[i] == patterns[j][len]) {
                len++;
                if (len == patterns[j].size()) {
                    if (!found[j]) {
                        found[j] = true;
                    } else { // ups, we found it repeated
                        found[j] = false;
                        deleted[j] = true;
                    }
                }
            }
            lens[j] = len;
        }
    }
    return found;
}

vector<string> compute_shifts(const string & w, const string & alph) {
    vector<string> res(alph.size());
    for (int shift = 0; shift < alph.size(); ++shift) {
        string s;
        for (char c: w) {
            int alph_idx = alph.find(c);
            int shift_letter = (alph_idx + shift) % alph.size();
            s += alph[shift_letter];
        }
        res[shift] = s;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);

    int tests;
    string alph, w, s;
    cin >> tests;

    while (tests--) {
        cin >> alph >> w >> s;
        vb solutions = kmp_search(s, compute_shifts(w, alph));
        vi r;
        for (int i = 0; i < solutions.size(); ++i) {
            if (solutions[i]) r.push_back(i);
        }

        if (r.empty()) cout << "no solution\n";
        else if (r.size() == 1) cout << "unique: " << r[0] << "\n";
        else {
            cout << "ambiguous: ";
            for (int i = 0; i < r.size(); ++i) {
                cout << r[i];
                if (i < r.size()-1) cout << " ";
            }
            cout << "\n";
        }
    }
}