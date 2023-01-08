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

/*
 * Implementation of: https://math.stackexchange.com/a/3266324
 *
 * 1) When two knights threaten each other they form a rectangle of size (2 x 3) or (3 x 2).
 *    Count how many (2 x 3) and (3 x 2) rectangles can we fit into (k x k) square.
 *    (k - 1)(k - 2) + (k - 2)(k - 1) = 2(k - 1)(k - 2)
 * 2) Within a single rectangle knights can be placed there in two ways.
 *    Thus, 2 * 2(k - 1)(k - 2) = 4(k - 1)(k - 2).
 * 3) First knight can be placed in any cell. Thus, there are k^2 possible cells for the first knight.
 *    The second knight can then be placed in k^2 - 1 cells. In total the possibilities are [k^2 * (k^2 - 1)] / 2.
 * 4) The answer is then:
 *    (possibilities to place knights) - (number of threatening positions) = [k^2 * (k^2 - 1)] / 2 - 4(k - 1)(k - 2)
 *
 * Rectangles are like this:
 * 1 1 1 0 0    1 1 0 0 0
 * 1 1 1 0 0    1 1 0 0 0
 * 0 0 0 0 0    1 1 0 0 0
 * 0 0 0 0 0    0 0 0 0 0
 * 0 0 0 0 0    0 0 0 0 0
 */

void Solve() {
  int n;
  cin >> n;

  for (ll k = 1; k <= n; ++k) {
    ll possibilities = (k * k) * (k * k - 1) / 2;
    ll threatening_positions = 4 * (k - 1) * (k - 2);
    ll ans = possibilities - threatening_positions;
    cout << ans << '\n';
  }

}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solve();
}
