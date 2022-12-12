#include <algorithm>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

using ll = long long;

unordered_map<string, unordered_set<string>> ParseInput() {
  unordered_map<string, unordered_set<string>> m;
  string s;
  while (getline(cin, s) && !s.empty()) {
    constexpr char delimiter = '-';
    string begin = s.substr(0, s.find(delimiter));
    string end = s.substr(s.find(delimiter) + 1);
    m[begin].insert(end);
    m[end].insert(begin);
  }
  return m;
}

int DFS(const string& u, unordered_map<string, unordered_set<string>>& m, const set<string>& visited, bool visit_twice) {
  if (u == "start" && visited.count(u)) return 0;
  if (u == "end") return 1;
  if (all_of(u.begin(), u.end(), [](auto c) { return islower(c); }) && visited.count(u)) {
    if (visit_twice) visit_twice = false;
    else return 0;
  }
  // Must create a copy for child calls.
  set<string> new_visited = visited;
  new_visited.insert(u);
  int paths = 0;
  for (const auto& v : m[u]) {
    paths += DFS(v, m, new_visited, visit_twice);
  }
  return paths;
}

int Solve() {
  unordered_map<string, unordered_set<string>> m = ParseInput();
  set<string> visited;
  const int paths = DFS("start", m, visited, true);
  return paths;
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}
