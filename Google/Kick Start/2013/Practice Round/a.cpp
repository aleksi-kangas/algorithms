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

int Solve() {
  int n;
  cin >> n;
  cin.ignore();
  vector<string> a(n);
  for (int i = 0; i < n; ++i) {
    getline(cin, a[i]);
  }
  int answer = 0;
  string last = a[0];
  for (int i = 1; i < n; ++i) {
    if (a[i] < last) {
      ++answer;
    } else {
      last = a[i];
    }
  }
  return answer;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int test_cases;
  cin >> test_cases;

  for (int test_case = 1; test_case <= test_cases; ++test_case) {
    auto answer = Solve();
    cout << "Case #" << test_case << ": " << answer << '\n';
  }
}
