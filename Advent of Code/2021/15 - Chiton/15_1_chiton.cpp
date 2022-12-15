#include <algorithm>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
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
#include <vector>

using namespace std;

using ll = long long;

using pq_elem = tuple<int, int, int>;

vector<vector<int>> ReadInput() {
  vector<vector<int>> m;
  string s;
  while (getline(cin, s) && !s.empty()) {
    vector<int> row;
    for (char c : s) {
      row.push_back(c - '0');
    }
    m.push_back(std::move(row));
  }
  return m;
}

int Solve() {
  const vector<vector<int>> risk = ReadInput();
  const int n = static_cast<int>(risk.size());
  const int m = static_cast<int>(risk[0].size());

  vector<vector<int>> total_risk(n, vector<int>(m, numeric_limits<int>::max()));
  total_risk[0][0] = 0;

  priority_queue<pq_elem, vector<pq_elem>, greater<>> pq;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      pq.emplace(total_risk[i][j], i, j);
    }
  }

  while (!pq.empty()) {
    const pq_elem elem = pq.top();
    pq.pop();
    // C++ priority queue implementation does not support 'change key' operation out of the box,
    // so I use 'lazy-deletion' by inserting a new element with the changed key,
    // and checking here if the element is already processed.
    if (total_risk[get<1>(elem)][get<2>(elem)] < get<0>(elem)) continue;
    constexpr int kDr[] = {-1, 0, 1, 0};
    constexpr int kDc[] = {0, 1, 0, -1};
    for (int i = 0; i < 4; ++i) {
      const int r = get<1>(elem) + kDr[i];
      const int c = get<2>(elem) + kDc[i];
      if (r < 0 || c < 0 || r == n || c == m) continue;
      const int new_total_risk = get<0>(elem) + risk[r][c];
      if (new_total_risk < total_risk[r][c]) {
        total_risk[r][c] = new_total_risk;
        pq.emplace(new_total_risk, r, c);
      }
    }
  }

  return total_risk[n - 1][m - 1];
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}
