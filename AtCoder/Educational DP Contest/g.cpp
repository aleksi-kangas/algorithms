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
#include <list>
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

void DFS(const vector<list<int>>& g, vector<bool>& visited, vector<int>& dp, int u) {
  visited[u] = true;
  for (const auto& v : g[u]) {
    if (!visited[v]) {
      DFS(g, visited, dp, v);
    }
    dp[u] = max(dp[u], dp[v] + 1);
  }
}

int Solve() {
  int n, m;
  cin >> n >> m;
  vector<list<int>> g(n, list<int>());
  for (int i = 0; i < m; ++i) {
    int x, y;
    cin >> x >> y;
    --x;
    --y;
    g[x].push_back(y);
  }
  // Recurrence: dp[u] = max(dp[u], dp[v...] + 1), where v is an immediate predecessor of u.
  // In other words, dp[u] is the length of the longest path starting at u.
  vector<int> dp(n, 0);
  vector<bool> visited(n, false);
  for (int i = 0; i < n; ++i) {
    DFS(g, visited, dp, i);
  }
  int answer = 0;
  for (int i = 0; i < n; ++i) {
    answer = max(answer, dp[i]);
  }
  return answer;
}

int main() {
  auto answer = Solve();
  cout << answer << '\n';
}
