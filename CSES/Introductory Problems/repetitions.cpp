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

void Solve() {
  string s;
  cin >> s;
  int n = static_cast<int>(s.size());

  int ans = 1;
  vector<int> dp(n, 1);
  for (int i = 1; i < n; ++i) {
    if (s[i - 1] == s[i]) {
      dp[i] += dp[i - 1];
    }
    ans = max(ans, dp[i]);
  }

  cout << ans << endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solve();
}
