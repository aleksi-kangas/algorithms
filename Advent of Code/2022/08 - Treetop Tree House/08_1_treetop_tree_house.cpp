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

vector<vector<int>> ReadInput() {
  vector<vector<int>> grid;
  string line;
  while (getline(cin, line) && !line.empty()) {
    vector<int> row;
    for (char c : line) {
      row.push_back(c - '0');
    }
    grid.push_back(row);
  }
  return grid;
}

bool IsVisibleLeft(const vector<vector<int>> &grid, int r, int c) {
  for (int j = c - 1; j >= 0; --j) {
    if (grid[r][c] <= grid[r][j]) {
      return false;
    }
  }
  return true;
}

bool IsVisibleRight(const vector<vector<int>> &grid, int r, int c) {
  for (int j = c + 1; j < grid[r].size(); ++j) {
    if (grid[r][c] <= grid[r][j]) {
      return false;
    }
  }
  return true;
}

bool IsVisibleUp(const vector<vector<int>> &grid, int r, int c) {
  for (int i = r - 1; i >= 0; --i) {
    if (grid[r][c] <= grid[i][c]) {
      return false;
    }
  }
  return true;
}

bool IsVisibleDown(const vector<vector<int>> &grid, int r, int c) {
  for (int i = r + 1; i < grid.size(); ++i) {
    if (grid[r][c] <= grid[i][c]) {
      return false;
    }
  }
  return true;
}

struct Visible {
  bool left = true;
  bool right = true;
  bool up = true;
  bool down = true;
};

int Solve() {
  const auto grid = ReadInput();
  const int n = static_cast<int>(grid.size());
  const int m = static_cast<int>(grid[0].size());

  vector<vector<Visible>> visible(n, vector<Visible>(m));
  for (int r = 0; r < n; ++r) {
    for (int c = 0; c < m; ++c) {
      visible[r][c].left = IsVisibleLeft(grid, r, c);
      visible[r][c].right = IsVisibleRight(grid, r, c);
      visible[r][c].up = IsVisibleUp(grid, r, c);
      visible[r][c].down = IsVisibleDown(grid, r, c);
    }
  }

  int answer = 0;
  for (int r = 0; r < n; ++r) {
    for (int c = 0; c < m; ++c) {
      if (visible[r][c].left || visible[r][c].right || visible[r][c].up ||
          visible[r][c].down) {
        ++answer;
      }
    }
  }
  return answer;
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}