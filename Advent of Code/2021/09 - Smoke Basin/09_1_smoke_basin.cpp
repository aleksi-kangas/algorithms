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

vector<vector<int>> ParseInput() {
  vector<vector<int>> m;
  string s;
  while (getline(cin, s) && !s.empty()) {
    vector<int> row;
    row.reserve(s.size());
    for (char c : s) {
      row.push_back(c - '0');
    }
    m.push_back(move(row));
  }
  return m;
}

int Solve() {
  vector<vector<int>> heights = ParseInput();
  int n = static_cast<int>(heights.size());
  int m = static_cast<int>(heights[0].size());
  int answer = 0;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      bool ok = true;
      if (i > 0) {
        ok &= heights[i][j] < heights[i - 1][j];
      }
      if (i < n - 1) {
        ok &= heights[i][j] < heights[i + 1][j];
      }
      if (j > 0) {
        ok &= heights[i][j] < heights[i][j - 1];
      }
      if (j < m - 1) {
        ok &= heights[i][j] < heights[i][j + 1];
      }
      if (ok) {
        answer += heights[i][j] + 1;
      }
    }
  }
  return answer;
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}
