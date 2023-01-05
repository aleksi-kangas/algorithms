#include <algorithm>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iomanip>
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
#include <variant>
#include <vector>

using namespace std;

using ll = long long;

bool Solve() {
  int n;
  cin >> n;
  cin.ignore();
  unordered_map<string, unordered_set<string>> adj;
  string start;
  for (int i = 0; i < n; ++i) {
    string s;
    getline(cin, s);
    auto pos = s.find(' ');
    string a = s.substr(0, pos);
    string b = s.substr(pos + 1);
    adj[a].insert(b);
    adj[b].insert(a);
    if (start.empty()) start = a;
  }

  unordered_map<string, int> color;
  for (const auto& [k, v] : adj) {
    color[k] = 0;
  }
  queue<string> q;
  q.push(start);
  while (!q.empty()) {
    string a = q.front();
    q.pop();
    for (const string& b : adj[a]) {
      if (color[b] == 0) {
        color[b] = color[a] == 1 ? 2 : 1;
        q.push(b);
      } else if (color[a] == color[b]) {
        return false;
      }
    }
  }
  return true;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int test_cases;
  cin >> test_cases;

  for (int test_case = 1; test_case <= test_cases; ++test_case) {
    auto answer = Solve();
    cout << "Case #" << test_case << ": " << (answer ? "Yes" : "No") << '\n';
  }
}
