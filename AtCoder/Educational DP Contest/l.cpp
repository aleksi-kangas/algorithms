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

ll DP(const vector<int>& a, vector<vector<ll>>& dp, int i, int j) {
  if (i > j) return 0;
  if (dp[i][j] > 0) return dp[i][j];
  const ll front = a[i] - DP(a, dp, i + 1, j);
  const ll back = a[j] - DP(a, dp, i, j - 1);
  dp[i][j] = max(front, back);
  return dp[i][j];
}

ll Solve() {
  int n;
  cin >> n;
  vector<int> a(n);
  for (int i = 0; i < n; ++i) {
    cin >> a[i];
  }
  vector<vector<ll>> dp(n, vector<ll>(n, -1));
  return DP(a, dp, 0, n - 1);
}

int main() {
  auto answer = Solve();
  cout << answer << '\n';
}
