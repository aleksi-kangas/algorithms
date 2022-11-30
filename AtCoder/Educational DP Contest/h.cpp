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
  int h, w;
  cin >> h >> w;

  vector<vector<int>> m(h, vector<int>(w));
  for (int i = 0; i < h; ++i) {
    string s;
    cin >> s;
    for (int j = 0; j < w; ++j) {
      m[i][j] = s[j] == '#' ? 1 : 0;
    }
  }
  vector<vector<ll>> dp(h, vector<ll>(w, 0));
  dp[0][0] = 1;
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      if (i + 1 < h && m[i + 1][j] == 0) {
        dp[i + 1][j] += dp[i][j];
        dp[i + 1][j] %= static_cast<ll>(1e9 + 7);
      }
      if (j + 1 < w && m[i][j + 1] == 0) {
        dp[i][j + 1] += dp[i][j];
        dp[i][j + 1] %= static_cast<ll>(1e9 + 7);
      }
    }
  }
  return dp[h - 1][w - 1];
}

int main() {
  auto answer = Solve();
  cout << answer << '\n';
}
