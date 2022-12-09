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
  vector<int> basin_sizes;
  basin_sizes.reserve(n * m);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      if (heights[i][j] == 9) {
        continue;
      }
      vector<vector<bool>> visited(n, vector<bool>(m, false));
      queue<pair<int, int>> q;
      q.emplace(i, j);
      int basin_size = 1;
      while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        if (x > 0 && heights[x][y] < heights[x - 1][y] && heights[x - 1][y] != 9) {
          if (!visited[x - 1][y]) {
            visited[x - 1][y] = true;
            q.emplace(x - 1, y);
            ++basin_size;
          }
        }
        if (x < n - 1 && heights[x][y] < heights[x + 1][y] && heights[x + 1][y] != 9) {
          if (!visited[x + 1][y]) {
            visited[x + 1][y] = true;
            q.emplace(x + 1, y);
            ++basin_size;
          }
        }
        if (y > 0 && heights[x][y] < heights[x][y - 1] && heights[x][y - 1] != 9) {
          if (!visited[x][y - 1]) {
            visited[x][y - 1] = true;
            q.emplace(x, y - 1);
            ++basin_size;
          }
        }
        if (y < m - 1 && heights[x][y] < heights[x][y + 1] && heights[x][y + 1] != 9) {
          if (!visited[x][y + 1]) {
            visited[x][y + 1] = true;
            q.emplace(x, y + 1);
            ++basin_size;
          }
        }
      }
      basin_sizes.push_back(basin_size);
    }
  }
  sort(basin_sizes.begin(), basin_sizes.end(), greater<>());
  return basin_sizes[0] * basin_sizes[1] * basin_sizes[2];
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}
