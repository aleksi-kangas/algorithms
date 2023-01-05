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

double Solve() {
  int v, d;
  cin >> v >> d;
  constexpr double alpha = 9.8;
  double r = (d * alpha) / (v * v);
  r = min(r, 1.0);
  r = max(r, -1.0);
  const double sin_theta = r;
  return asin(sin_theta) * 90.0 / M_PI;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int test_cases;
  cin >> test_cases;

  for (int test_case = 1; test_case <= test_cases; ++test_case) {
    auto answer = Solve();
    cout << fixed << setprecision(6);
    cout << "Case #" << test_case << ": " << answer << '\n';
  }
}
