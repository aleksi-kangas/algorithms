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

vector<int> Solve() {
  int n;
  cin >> n;
  vector<int> s(n);
  int even_count = 0, odd_count = 0;
  for (int i = 0; i < n; ++i) {
    cin >> s[i];
    if (s[i] % 2 == 0)
      ++even_count;
    else
      ++odd_count;
  }
  vector<int> even;
  even.reserve(even_count);
  vector<int> odd;
  odd.reserve(odd_count);
  for (int x : s) {
    if (x % 2 == 0)
      even.push_back(x);
    else
      odd.push_back(x);
  }
  sort(even.begin(), even.end(), greater<>());
  sort(odd.begin(), odd.end());
  int e = 0, o = 0;
  for (int i = 0; i < n; ++i) {
    if (s[i] % 2 == 0)
      s[i] = even[e++];
    else
      s[i] = odd[o++];
  }
  return s;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int test_cases;
  cin >> test_cases;

  for (int test_case = 1; test_case <= test_cases; ++test_case) {
    auto answer = Solve();
    cout << "Case #" << test_case << ": ";
    for (int x : answer) cout << x << ' ';
    cout << '\n';
  }
}
