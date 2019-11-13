#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <deque>

using namespace std;

typedef pair<int, int> ii;

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

void solve2(const vector<string> & ss) {
    if (ss.size() == 1) { cout << ss[0] << "\n"; return; }

    string complete;
    vector<int> begins(ss.size());
    int previous = 0;
    for (int i = 0; i < ss.size(); ++i) {
        begins[i] = previous;
        complete += ss[i] + get_delimiter(i);
        previous += ss[i].size() + 1;
    }
    vector<int> sa = suffix_array(complete); vector<int> ss_lcp = lcp(complete, sa);
    int bound = ss.size() / 2;

    unordered_map<int, int> strings; unordered_set<int> max_pre_idxs; deque<ii> queue;
    int l = 0, r = 0, max = 0;
    while (r < ss_lcp.size()) {
        int pre1_str = string_belonging(begins, sa[r]);
        int pre2_str = string_belonging(begins, sa[r+1]);

        // If new item would increase the amount of strings, we have remove from left of queue
        if (strings.find(string_belonging(begins, sa[r+1])) == strings.end()) {
            while (l <= r && strings.size() == bound+1) {
                ii q_front = queue.front();
                if (q_front.second == l) {
                    if (max == q_front.first) max_pre_idxs.insert(q_front.second);
                    else if (max < q_front.first) {
                        max_pre_idxs.clear(); max_pre_idxs.insert(q_front.second); max = q_front.first;
                    }
                    queue.pop_front();
                }
                int pre_str = string_belonging(begins, sa[l]);
                strings[pre_str] -= 1;
                if (strings[pre_str] == 0) strings.erase(pre_str);
                l++;
            }
        }

        if (strings.find(pre1_str) != strings.end()) strings[pre1_str] += 1;
        else strings[pre1_str] = 1;
        if (strings.find(pre2_str) != strings.end()) strings[pre2_str] += 1;
        else strings[pre2_str] = 1;

        // Update from the rear with new value in case its lower
        while (!queue.empty() && queue.back().first > ss_lcp[r]) {
            ii q_back = queue.back();
            queue.pop_back();
            if (strings.size() < bound+1) continue;
            if (max == q_back.first) max_pre_idxs.insert(q_back.second);
            else if (max < q_back.first) {
                max_pre_idxs.clear(); max_pre_idxs.insert(q_back.second); max = q_back.first;
            }
        }
        if (pre1_str != pre2_str) {  // Im not comparing prefixes of the same str
            if (queue.empty()) {
                queue.push_back(ii(ss_lcp[r], r));
            } else queue.push_back(ii(ss_lcp[r], r));
        }

        r++;
    }
    if (strings.size() >= bound+1) {
        for (ii d: queue) {
            if (max == d.first) max_pre_idxs.insert(d.second);
            else if (max < d.first) {  // we found a new maximum
                max_pre_idxs.clear(); max_pre_idxs.insert(d.second); max = d.first;
            }
        }
    }
    if (max == 0) cout << "?\n";
    else {
        set<string> result;
        for (int index: max_pre_idxs) result.insert(complete.substr(sa[index], ss_lcp[index]));
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