#include <iostream>
#include <vector>
#include <set>
#include <map>

using namespace std;

// Suffix Array O(nlogn)
// s.push('$');
vector<int> suffix_array(string &s){
  int n = s.size(), alph = 256;
  vector<int> cnt(max(n, alph)), p(n), c(n);

  for(auto c : s) cnt[c]++;
  for(int i = 1; i < alph; i++) cnt[i] += cnt[i - 1];
  for(int i = 0; i < n; i++) p[--cnt[s[i]]] = i;
  for(int i = 1; i < n; i++)
    c[p[i]] = c[p[i - 1]] + (s[p[i]] != s[p[i - 1]]);

  vector<int> c2(n), p2(n);

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
vector<int> lcp(string &s, vector<int> &p){
  int n = s.size();
  vector<int> ans(n - 1), pi(n);
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
int string_belonging(vector<int> & begins, int index) {
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

void solve(vector<string> & ss) {
    if (ss.size() == 1) {
        cout << ss[0] << "\n";
        return;
    }

    string complete;
    vector<int> begins(ss.size());
    int previous = 0;
    for (int i = 0; i < ss.size(); ++i) {
        begins[i] = previous;
        complete += ss[i] + get_delimiter(i);
        previous += ss[i].size() + 1;
    }
    vector<int> sa = suffix_array(complete);
    vector<int> ss_lcp = lcp(complete, sa);

    // Build a summary of the solution
    map<string, set<int>> solutions;
    for (int i = 0; i < ss_lcp.size(); ++i) {
        int prefix_length = ss_lcp[i];
        if (prefix_length == 0) continue;

        int prefix1_idx = sa[i+1]; int prefix2_idx = sa[i];
        int prefix1_str = string_belonging(begins, prefix1_idx);
        int prefix2_str = string_belonging(begins, prefix2_idx);
        string prefix = complete.substr(prefix1_idx, prefix_length);
        // shorter prefixes are also valid
        for (int j = 0; j < prefix.size(); ++j) {
            string prefix_shortened = prefix.substr(0, prefix.size()-j);
            solutions[prefix_shortened].insert(prefix1_str);
            solutions[prefix_shortened].insert(prefix2_str);
        }
    }

    // Get a final solution
    int longest = 0;
    for (pair<string, set<int>> s: solutions)
        if (s.second.size() > ss.size()/2)
            longest = max(longest, int(s.first.size()));

    if (longest == 0) {
        cout << "?\n";
    } else {
        for (pair<string, set<int>> s: solutions)
            if (s.first.size() == longest && s.second.size() > ss.size()/2)
                cout << s.first << "\n";
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
        solve(ss);
    }
}