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

ll Solve() {
  int n, W;
  cin >> n >> W;
  vector<int> w(n);
  vector<int> v(n);
  for (int i = 0; i < n; ++i) {
    cin >> w[i] >> v[i];
  }
  // Recurrence: dp[w] = max(dp[w], v[i] + dp[w - w[i])
  vector<ll> dp(W + 1);
  for (int i = 0; i < n; ++i) {
    for (int j = W; j >= w[i]; --j) {
      dp[j] = max(dp[j], v[i] + dp[j - w[i]]);
    }
  }
  return dp[W];
}

int main() {
  auto answer = Solve();
  cout << answer << '\n';
}
