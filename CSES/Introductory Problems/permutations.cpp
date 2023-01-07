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
  int n;
  cin >> n;

  if (n == 1) {
    cout << 1 << endl;
    return;
  }

  if (n <= 3) {
    cout << "NO SOLUTION" << endl;
    return;
  }

  for (int i = 2; i <= n; i += 2) {
    cout << i << ' ';
  }
  for (int i = 1; i <= n; i += 2) {
    cout << i << ' ';
  }

  cout << endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solve();
}
