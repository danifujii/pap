#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <deque>
#include <algorithm>

using namespace std;

#define forr(i,a,b) for(int i=(a); i <(b); i++)
#define forn(i,n) forr(i,0,n)
#define sz(v) (int)(v).size()

typedef pair<int, int> ii;
typedef vector<int> vi;

typedef const int&(*op)(const int&, const int&);  // asociativa, idempotente
template<op op>

struct st{
    vector<vi> t;

    st(const vi &A) { // O(n.logn)
        int n = sz(A);
        int K = 31 - __builtin_clz(n)+1;
        t.assign(K, vi(n));
        forn(i,n) t[0][i] = A[i];
        forr(k,1,K) forn(i, n-(1<<k)+1)
            t[k][i] = op(t[k-1][i], t[k-1][i+(1<<(k-1))]);
    }

    int query(int l, int r) {  // O(1), [l, r)
        int k = 31 - __builtin_clz(r-l);
        return op(t[k][l], t[k][r-(1<<k)]);
    }
};

// Suffix Array O(nlogn)
// s.push('$');
vi suffix_array(string &s){
    int n = s.size(), alph = 256;
    vi cnt(max(n, alph)), p(n), c(n);

    for(auto c : s) cnt[c]++;
    for(int i = 1; i < alph; i++) cnt[i] += cnt[i - 1];
    for(int i = 0; i < n; i++) p[--cnt[s[i]]] = i;
    for(int i = 1; i < n; i++)
    c[p[i]] = c[p[i - 1]] + (s[p[i]] != s[p[i - 1]]);

    vi c2(n), p2(n);

    for(int k = 0; (1 << k) < n; k++){
    int classes = c[p[n - 1]] + 1;
    fill(cnt.begin(), cnt.begin() + classes, 0);

    for(int i = 0; i < n; i++) p2[i] = (p[i] - (1 << k) + n)%n;
    for(int i = 0; i < n; i++) cnt[c[i]]++;
    for(int i = 1; i < classes; i++) cnt[i] += cnt[i - 1];
    for(int i = n - 1; i >= 0; i--) p[--cnt[c[p2[i]]]] = p2[i];

    c2[p[0]] = 0;
    for(int i = 1; i < n; i++){
        pair<int, int> b1 = {c[p[i]], c[(p[i] + (1 << k))%n]};
        pair<int, int> b2 = {c[p[i - 1]], c[(p[i - 1] + (1 << k))%n]};
        c2[p[i]] = c2[p[i - 1]] + (b1 != b2);
    }

    c.swap(c2);
    }
    return p;
}

// Longest Common Prefix with SA O(n)
vi lcp(string &s, vi &p){
    int n = s.size();
    vi ans(n - 1), pi(n);
    for(int i = 0; i < n; i++) pi[p[i]] = i;

    int lst = 0;
    for(int i = 0; i < n - 1; i++){
    if(pi[i] == n - 1) continue;
    while(s[i + lst] == s[p[pi[i] + 1] + lst]) lst++;

    ans[pi[i]] = lst;
    lst = max(0, lst - 1);
    }

    return ans;
}

char get_delimiter(int index) {
    if (index > 63) index = index % 63;
    return char(35 + index);
}

// get to which string (index) this index belongs to
int string_belonging(vi & begins, int index) {
    int left = 0;
    int right = begins.size() - 1;
    while (left <= right) {
        int middle = (right + left) / 2;
        if (begins[middle] == index) return middle;
        else if (begins[middle] < index) {
            // no more elements to right or it is not in the range of the next one
            if (middle == begins.size() - 1 || index < begins[middle+1])
                return middle;
            else left = middle + 1;
        } else {
            if (middle == 0) return middle;
            else right = middle - 1;
        }
    }
}

vi get_limits(const vector<string> & ss) {
   vi begins(ss.size());
   int previous = 0;
   for (int i = 0; i < ss.size(); ++i) {
       begins[i] = previous;
       previous += ss[i].size() + 1;
   }
   return begins;
}

string get_complete_string(const vector<string> & ss) {
   string complete;
   for (int i = 0; i < ss.size(); ++i) {
       complete += ss[i] + get_delimiter(i);
   }
   return complete;
}

void solve2(const vector<string> & ss) {
    if (ss.size() == 1) { cout << ss[0] << "\n"; return; }

    // Init data structures
    string complete = get_complete_string(ss);
    vi begins = get_limits(ss); vi sa = suffix_array(complete); vi ss_lcp = lcp(complete, sa);
    st<min> rmq(ss_lcp);
    int bound = ss.size() / 2 + 1, l = 0, r = 0, max_len = 0;
    unordered_map<int, int> strings; set<int> lcp_idx;

    int pre1_str = string_belonging(begins, sa[0]); strings[pre1_str] += 1;

    while (r < ss_lcp.size()) {
        int pre1_str = string_belonging(begins, sa[r]);
        int pre2_str = string_belonging(begins, sa[r+1]);

        strings[pre2_str] += 1;
        // Added new element, reduce it to bound size or
        // move left to the smallest possible window having bound elements
        while (l < r && strings.size() >= bound) {
            int l_str = string_belonging(begins, sa[l]);
            if (strings.size() == bound && strings[l_str] == 1) break;  // dont break the bound limit
            strings[l_str] -= 1;
            if (strings[l_str] == 0) strings.erase(l_str);
            l += 1;

            if (strings.size() == bound) {
                int window_min = rmq.query(l, r+1);
                if (window_min > 0 && window_min >= max_len) {
                    if (window_min > max_len)
                        lcp_idx.clear();
                    lcp_idx.insert(r);
                    max_len = window_min;
                }
            }
        }
        r += 1;
   }

    if (max_len == 0) {
        cout << "?\n";
    } else {
        set<string> result;
        for (int i: lcp_idx) {
            result.insert(complete.substr(sa[i], max_len));
        }
        for (string s: result) cout << s << "\n";
    }
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0);

    bool first = true;
    int n; string s;
    while (true) {
        cin >> n;
        if (n == 0) break;
        if (!first) {
            cout << "\n";
        } else first = false;

        vector<string> ss(n);
        for (int i = 0; i < n; ++i) {
            cin >> s;
            ss[i] = s;
        }
        solve2(ss);
    }
}
