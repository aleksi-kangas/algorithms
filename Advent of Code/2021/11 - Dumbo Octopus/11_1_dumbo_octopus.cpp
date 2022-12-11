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
    for (char c : s) {
      row.push_back(c - '0');
    }
    m.push_back(move(row));
  }
  return m;
}

void Flash(vector<vector<int>>& e, vector<vector<bool>>& flashed, int y, int x) {
  int n = static_cast<int>(e.size());
  int m = static_cast<int>(e[0].size());
  for (int i = y - 1; i <= y + 1; ++i) {
    for (int j = x - 1; j <= x + 1; ++j) {
      if ((i == y && j == x) || i < 0 || j < 0 || i == n || j == m) {
        continue;
      }
      ++e[i][j];
    }
  }

  for (int i = y - 1; i <= y + 1; ++i) {
    for (int j = x - 1; j <= x + 1; ++j) {
      if ((i == y && j == x) || i < 0 || j < 0 || i == n || j == m) {
        continue;
      }
      if (e[i][j] > 9 && !flashed[i][j]) {
        flashed[i][j] = true;
        Flash(e, flashed, i, j);
      }
    }
  }
}

int Solve() {
  int flashes = 0;
  vector<vector<int>> e = ParseInput();
  int n = static_cast<int>(e.size());
  int m = static_cast<int>(e[0].size());
  for (int s = 1; s <= 100; ++s) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        ++e[i][j];
      }
    }
    vector<vector<bool>> flashed(n, vector<bool>(m, false));
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        if (e[i][j] > 9 && !flashed[i][j]) {
          flashed[i][j] = true;
          Flash(e, flashed, i, j);
        }
      }
    }
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        if (flashed[i][j]) {
          ++flashes;
          e[i][j] = 0;
        }
      }
    }
  }
  return flashes;
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}
