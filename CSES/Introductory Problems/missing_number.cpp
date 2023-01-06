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
  ll n;
  cin >> n;

  vector<ll> a(n - 1);
  for (int i = 0; i < n - 1; ++i) {
    cin >> a[i];
  }

  ll sum = n * (n + 1) / 2;
  for (int i = 0; i < n - 1; ++i) {
    sum -= a[i];
  }
  cout << sum << endl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solve();
}
