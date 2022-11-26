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
  int n, k;
  cin >> n >> k;
  vector<int> a(n);
  for (int i = 0; i < n; ++i) {
    cin >> a[i];
  }
  // Recurrence: dp[i] = min(dp[i], dp[i - j] + cost(i - j, i))
  // where cost(i, j) = abs(a[i] - a[i - j]) and j = 1...k.
  vector<int> dp(n, numeric_limits<int>::max());
  dp[0] = 0;
  for (int i = 1; i < n; ++i) {
    for (int j = 1; j <= min(i, k); ++j) {
      dp[i] = min(dp[i], dp[i - j] + abs(a[i] - a[i - j]));
    }
  }
  return dp[n - 1];
}

int main() {
  auto answer = Solve();
  cout << answer << '\n';
}
