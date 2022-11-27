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

int Solve() {
  int n;
  cin >> n;
  vector<int> a(n);
  vector<int> b(n);
  vector<int> c(n);
  for (int i = 0; i < n; ++i) {
    cin >> a[i] >> b[i] >> c[i];
  }
  // Recurrence: dp[i][j] = max(dp[i - 1][k] + abc[i]]),
  // where k = 1...3 and k != j.
  vector<vector<int>> dp(n, vector<int>(3));
  dp[0][0] = a[0];
  dp[0][1] = b[0];
  dp[0][2] = c[0];
  for (int i = 1; i < n; ++i) {
    dp[i][0] = max(dp[i - 1][1], dp[i - 1][2]) + a[i];
    dp[i][1] = max(dp[i - 1][0], dp[i - 1][2]) + b[i];
    dp[i][2] = max(dp[i - 1][0], dp[i - 1][1]) + c[i];
  }
  return max({dp[n - 1][0], dp[n - 1][1], dp[n - 1][2]});
}

int main() {
  auto answer = Solve();
  cout << answer << '\n';
}
