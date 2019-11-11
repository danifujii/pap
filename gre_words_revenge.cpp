#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <string.h>
#include <assert.h>
#include <algorithm>

using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

struct AhoCorasick {
	enum {alpha = 26, first = '0'};
	struct Node {
		// (nmatches is optional)
		int back, next[alpha], start = -1, end = -1, nmatches = 0;
		Node(int v) { memset(next, v, sizeof(next)); }
	};
	vector<Node> N;
	vector<int> backp;
	void insert(string& s, int j) {
		assert(!s.empty());
		int n = 0;
		trav(c, s) {
			int& m = N[n].next[c - first];
			if (m == -1) { n = m = sz(N); N.emplace_back(-1); }
			else n = m;
		}
		if (N[n].end == -1) N[n].start = j;
		backp.push_back(N[n].end);
		N[n].end = j;
		N[n].nmatches++;
	}
	AhoCorasick(vector<string>& pat) {
		N.emplace_back(-1);
		rep(i,0,sz(pat)) insert(pat[i], i);
		N[0].back = sz(N);
		N.emplace_back(0);

		queue<int> q;
		for (q.push(0); !q.empty(); q.pop()) {
			int n = q.front(), prev = N[n].back;
			rep(i,0,alpha) {
				int &ed = N[n].next[i], y = N[prev].next[i];
				if (ed == -1) ed = y;
				else {
					N[ed].back = y;
					(N[ed].end == -1 ? N[ed].end : backp[N[ed].start])
						= N[y].end;
					N[ed].nmatches += N[y].nmatches;
					q.push(ed);
				}
			}
		}
	}
	int find(string word) {
		int n = 0;
		int count = 0;
		trav(c, word) {
			n = N[n].next[c - first];
			count += N[n].nmatches;
		}
		return count;
	}
};

string shift(string & s, int shifts) {
    if (s.size() > shifts)
        return s.substr(shifts, s.size()-shifts) + s.substr(0, shifts);
    else {
        // simulate, unfortunately
        for (int i = 0; i < shifts; ++i)
            s = s.substr(1, s.size()-1) + s.substr(0, 1);
        return s;
    }
}

int nodes_limit = 1500;

int main() {
    ios::sync_with_stdio(0); cin.tie(0);

    string s;
    int t, l; cin >> t;
    for (int i = 1; i <= t; ++i) {
        cout << "Case #" << i << ":\n";
        cin >> l;
        int shifts = 0, nodes = 0;
        unordered_set<string> all_words;
        vector<string> complete_words; vector<string> latest_words;
        AhoCorasick pri = AhoCorasick(complete_words); AhoCorasick sec = AhoCorasick(latest_words);
        bool rebuild_pri = false, rebuild_sec = false;

        while (l--) {
            cin >> s;
            char op = s[0]; s = s.substr(1, s.size()-1); s = shift(s, shifts); // string cleanup
            if (op == '+') {  // add pattern
                if (all_words.find(s) != all_words.end()) continue; // pattern already found
                auto it = find(complete_words.begin(), complete_words.end(), s);
                if (it == complete_words.end()) {
                    all_words.insert(s);
                    complete_words.push_back(s);
                    latest_words.push_back(s);
                    rebuild_sec = true;
                    nodes += s.size();
                    if (nodes > nodes_limit) rebuild_pri = true;
                }
            } else {  // query time
                if (rebuild_pri) {
                    pri = AhoCorasick(complete_words);
                    latest_words.clear();
                    sec = AhoCorasick(latest_words);
                    nodes = 0; rebuild_pri = false; rebuild_sec = false;
                } else if (rebuild_sec) {
                    sec = AhoCorasick(latest_words); rebuild_sec = false;
                }

                int pri_matches = pri.find(s);
                int sec_matches = sec.find(s);
                int res = pri_matches + sec_matches;
                cout << res << "\n";
                shifts = res;
            }
        }
    }
}