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
  int n;
  cin >> n;

  if (n % 4 == 0) {
    cout << "YES" << '\n';
    /*
     * a b c d
     * 1 2 3 4
     * 5 6 7 8
     * (a + d) == (b + c) when n % 4 == 0
     */
    cout << (n / 2) << '\n';
    for (int i = 1; i <= n - 3; i += 4) {
      cout << i << ' ' << (i + 3) << ' ';
    }
    cout << '\n';
    cout << (n / 2) << '\n';
    for (int i = 1; i <= n - 3; i += 4) {
      cout << (i + 1) << ' ' << (i + 2) << ' ';
    }
  } else if (n % 4 == 3) {
    cout << "YES" << '\n';
    /*
     * a b c
     * 1 2 3    (1 + 2) == 3
     * 5 6 7    (5 + 6) != 7
     * 9 10 11  (9 + 10) != 11
     */
    cout << (n / 2 + 1) << '\n';
    cout << '1' << ' ' << '2' << ' ';
    for (int i = 4; i <= n - 3; i += 4) {
      cout << i << ' ' << (i + 3) << ' ';
    }
    cout << '\n';
    cout << (n / 2) << '\n';
    cout << '3' << ' ';
    for (int i = 4; i <= n - 3; i += 4) {
      cout << (i + 1) << ' ' << (i + 2) << ' ';
    }
  } else {
    cout << "NO" << '\n';
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solve();
}
