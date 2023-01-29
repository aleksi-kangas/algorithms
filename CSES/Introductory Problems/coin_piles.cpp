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
  ll a, b;
  cin >> a >> b;

  /*
   * 1) Sum needs to be divisible by three.
   * 2) There needs to be enough coins in the smaller pile to empty the larger pile,
   *    i.e. the smaller pile must have at least half of the amount of coins in the larger pile.
   */

  if ((a + b) % 3 == 0 && min(a, b) * 2 >= max(a, b)) {
    cout << "YES" << '\n';
  } else {
    cout << "NO" << '\n';
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int test_cases = 0;
  cin >> test_cases;

  while (test_cases--) {
    Solve();
  }
}
