#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <queue>

using namespace std;

const int INT_INF = -1;
constexpr char min_alphabet = '0';
constexpr char max_alphabet = '1';
const int root_state = 0;

struct ahoState {
  unordered_map<char, int> go_to;
  bool leaf;
  int parent;
  int fail;
  set<string> words;
  int output;

  ahoState(int parent= INT_INF)
  : parent(parent), fail(0), output(INT_INF), leaf(false) {}
};

struct ahoCorasick {
  vector<ahoState> state_machine;
  ahoCorasick(const set<string>&);
  int find_keywords(const string&);
};

ahoCorasick::ahoCorasick(const set<string>& keywords) {
  state_machine.emplace_back();

  for (const string & w: keywords) {
    int state_index = root_state;
    for (char c : w) {
      if (state_machine[state_index].go_to.count(c) == 0) {
        state_machine[state_index].go_to[c] = state_machine.size();
        state_machine.emplace_back(state_index);
      }
      state_index = state_machine[state_index].go_to[c];
    }
    state_machine[state_index].leaf = true;
    state_machine[state_index].words.insert(w);
  }

  for (char c = min_alphabet; c <= max_alphabet; ++c)
    if (state_machine[root_state].go_to.count(c) == 0)
      state_machine[root_state].go_to[c] = root_state;

  auto states = queue<int>();

  for (auto it = state_machine[root_state].go_to.begin(); it != state_machine[root_state].go_to.end(); ++it)
    if (it->second != root_state) states.push(it->second);

  while (states.size() != 0) {
    int state_index = states.front();
    states.pop();

    for (auto it = state_machine[state_index].go_to.begin(); it != state_machine[state_index].go_to.end(); ++it) {
      int fail_index = state_machine[state_index].fail;

      //Falla hasta encontrar un camino por c
      while(state_machine[fail_index].go_to.count(it->first) == 0) fail_index = state_machine[fail_index].fail;
      fail_index = state_machine[fail_index].go_to[it->first];
      state_machine[it->second].fail = fail_index;

      if (state_machine[fail_index].leaf) state_machine[it->second].output = fail_index;
      else state_machine[it->second].output = state_machine[fail_index].output;

      states.push(it->second);
    }
  }
}

int ahoCorasick::find_keywords(const string& x) {
  int matches = 0;

  int current_state = root_state;
  for (char c : x) {
    while (state_machine[current_state].go_to.count(c) == 0) current_state = state_machine[current_state].fail;
    current_state = state_machine[current_state].go_to[c];

    if (state_machine[current_state].leaf) matches += state_machine[current_state].words.size();

    int output_index = current_state;
    while (state_machine[output_index].output != INT_INF){
      output_index = state_machine[output_index].output;
      matches += state_machine[output_index].words.size();
    }
  }
  return matches;
}

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

int nodes_limit = 500;

int main() {
    ios::sync_with_stdio(0); cin.tie(0);

    string s;
    int t, l; cin >> t;
    for (int i = 1; i <= t; ++i) {
        cout << "Case #" << i << ":\n";
        cin >> l;
        int shifts = 0, nodes = 0;
        set<string> all_words; set<string> latest_words;
        ahoCorasick pri = ahoCorasick(all_words); ahoCorasick sec = ahoCorasick(latest_words);
        bool rebuild_pri = false, rebuild_sec = false;

        while (l--) {
            cin >> s;
            char op = s[0]; s = s.substr(1, s.size()-1); s = shift(s, shifts); // string cleanup
            if (op == '+') {  // add pattern
                if (all_words.find(s) == all_words.end()) {
                    all_words.insert(s);
                    latest_words.insert(s);
                    rebuild_sec = true;
                    nodes += s.size();
                    if (nodes > nodes_limit) rebuild_pri = true;
                }
            } else {  // query time
                if (rebuild_pri) {
                    pri = ahoCorasick(all_words);
                    latest_words.clear();
                    sec = ahoCorasick(latest_words);
                    nodes = 0; rebuild_pri = false; rebuild_sec = false;
                } else if (rebuild_sec) {
                    sec = ahoCorasick(latest_words); rebuild_sec = false;
                }

                int pri_matches = pri.find_keywords(s);
                int sec_matches = sec.find_keywords(s);
                int res = pri_matches + sec_matches;
                cout << res << "\n";
                shifts = res;
            }
        }
    }
}