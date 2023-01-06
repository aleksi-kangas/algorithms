#include <algorithm>
#include <bitset>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

using ll = long long;

void Solve() {
  ll n;
  cin >> n;

  while (true) {
    cout << n << ' ';
    if (n == 1) {
      return;
    } else if (n % 2 == 0) {
      n /= 2;
    } else {
      n *= 3;
      ++n;
    }
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solve();
}
