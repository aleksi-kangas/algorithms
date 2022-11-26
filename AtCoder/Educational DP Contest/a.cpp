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
  for (int i = 0; i < n; ++i) {
    cin >> a[i];
  }
  // Recurrence: dp[i] = min(dp[i - 1] + cost(i-1, i), dp[i - 2] + cost(i-2, i)),
  // where cost(i, j) = abs(a[i] - a[j]).
  vector<int> dp(n);
  dp[0] = 0;
  dp[1] = abs(a[1] - a[0]);
  for (int i = 2; i < n; ++i) {
    dp[i] = min(dp[i - 1] + abs(a[i] - a[i - 1]), dp[i - 2] + abs(a[i] - a[i - 2]));
  }
  return dp[n - 1];
}

int main() {
  auto answer = Solve();
  cout << answer << '\n';
}
