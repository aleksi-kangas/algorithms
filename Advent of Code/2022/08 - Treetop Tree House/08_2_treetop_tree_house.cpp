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

int CountVisibleLeft(const vector<vector<int>> &grid, int r, int c) {
  int count = 0;
  for (int j = c - 1; j >= 0; --j) {
    if (grid[r][c] <= grid[r][j]) {
      ++count;
      return count;
    }
    ++count;
  }
  return count;
}

int CountVisibleRight(const vector<vector<int>> &grid, int r, int c) {
  int count = 0;
  for (int j = c + 1; j < grid[r].size(); ++j) {
    if (grid[r][c] <= grid[r][j]) {
      ++count;
      return count;
    }
    ++count;
  }
  return count;
}

int CountVisibleUp(const vector<vector<int>> &grid, int r, int c) {
  int count = 0;
  for (int i = r - 1; i >= 0; --i) {
    if (grid[r][c] <= grid[i][c]) {
      ++count;
      return count;
    }
    ++count;
  }
  return count;
}

int CountVisibleDown(const vector<vector<int>> &grid, int r, int c) {
  int count = 0;
  for (int i = r + 1; i < grid.size(); ++i) {
    if (grid[r][c] <= grid[i][c]) {
      return count;
    }
    ++count;
  }
  return count;
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
  int answer = 0;
  for (int r = 0; r < n; ++r) {
    for (int c = 0; c < m; ++c) {
      int score = 1;
      score *= CountVisibleLeft(grid, r, c);
      score *= CountVisibleRight(grid, r, c);
      score *= CountVisibleUp(grid, r, c);
      score *= CountVisibleDown(grid, r, c);
      answer = max(answer, score);
    }
  }
  return answer;
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}