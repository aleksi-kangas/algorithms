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
  int total_value = 0;
  for (int i = 0; i < n; ++i) {
    cin >> w[i] >> v[i];
    total_value += v[i];
  }
  // Recurrence: dp[v] = min(dp[v], v[i] + dp[j - v[i])
  vector<ll> dp(total_value + 1, 1e18 + 10);
  dp[0] = 0;
  for (int i = 0; i < n; ++i) {
    for (int j = total_value; j >= v[i]; --j) {
      dp[j] = min(dp[j], dp[j - v[i]] + w[i]);
    }
  }
  ll answer = 0;
  for (ll i = 0; i <= total_value; ++i) {
    if (dp[i] <= W) {
      answer = i;
    }
  }
  return answer;
}

int main() {
  auto answer = Solve();
  cout << answer << '\n';
}
