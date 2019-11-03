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
  vector<int> words;  // Si bien usamos aristas de output, es necesario tener un vector ya que podría haber patrones repetidos.
  int output;

  ahoState(int parent= INT_INF)
  : parent(parent), fail(0), output(INT_INF), leaf(false) {}
};

struct ahoCorasick {
  vector<ahoState> state_machine;
  ahoCorasick(const vector<string>&);
  int find_keywords(const string&);
};

ahoCorasick::ahoCorasick(const vector<string>& keywords) {
  state_machine.emplace_back();

  for (int i = 0; i < keywords.size(); ++i) {
    int state_index = root_state;
    for (char c : keywords[i]) {
      if (state_machine[state_index].go_to.count(c) == 0) {
        state_machine[state_index].go_to[c] = state_machine.size();
        state_machine.emplace_back(state_index);
      }
      state_index = state_machine[state_index].go_to[c];
    }
    state_machine[state_index].leaf = true;
    state_machine[state_index].words.push_back(i);
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

    if (state_machine[current_state].leaf) for (auto word_id : state_machine[current_state].words) matches++;

    int output_index = current_state;
    while (state_machine[output_index].output != INT_INF){
      output_index = state_machine[output_index].output;
      for (auto word_id : state_machine[output_index].words) matches++;
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

int main() {
    ios::sync_with_stdio(0); cin.tie(0);

    string s;
    int t, l; cin >> t;
    for (int i = 1; i <= t; ++i) {
        cout << "Case #" << i << ":\n";
        cin >> l;
        int shifts = 0;
        set<string> words;
        while (l--) {
            cin >> s;
            char op = s[0]; s = s.substr(1, s.size()-1); s = shift(s, shifts); // string cleanup
            if (op == '+') {  // add pattern
                words.insert(s);
            } else {  // query time
                ahoCorasick aho = ahoCorasick(vector<string>(words.begin(), words.end()));
                int res = aho.find_keywords(s);
                cout << res << "\n";
                shifts++;
            }
        }
    }
}