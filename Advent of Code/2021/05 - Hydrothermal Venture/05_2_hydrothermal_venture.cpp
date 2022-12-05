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

struct Coordinates {
  int x = 0;
  int y = 0;

  bool operator<(const Coordinates& coordinates) const {
    if (x == coordinates.x) {
      return y < coordinates.y;
    }
    return x < coordinates.x;
  }
};

vector<pair<Coordinates, Coordinates>> ParseInput() {
  vector<pair<Coordinates, Coordinates>> coordinate_pairs;
  string s;
  while (getline(cin, s) && !s.empty()) {
    int i = static_cast<int>(s.find_first_of(','));
    int j = static_cast<int>(s.find_first_of(' '));
    Coordinates first;
    try {
      first.x = stoi(s.substr(0, i));
      first.y = stoi(s.substr(i + 1, j));
    } catch (const invalid_argument& invalid_argument) {
      return {};
    }

    i = static_cast<int>(s.find_last_of(' '));
    j = static_cast<int>(s.find_last_of(','));
    Coordinates second;
    try {
      second.x = stoi(s.substr(i + 1, j));
      second.y = stoi(s.substr(j + 1));
    } catch (const invalid_argument& invalid_argument) {
      return {};
    }
    coordinate_pairs.emplace_back(first, second);
  }
  return coordinate_pairs;
}

bool IsHorizontal(const pair<Coordinates, Coordinates>& coordinate_pair) {
  return coordinate_pair.first.y == coordinate_pair.second.y;
}

bool IsVertical(const pair<Coordinates, Coordinates>& coordinate_pair) {
  return coordinate_pair.first.x == coordinate_pair.second.x;
}

bool IsDiagonal(const pair<Coordinates, Coordinates>& coordinate_pair) {
  int dx = coordinate_pair.second.x - coordinate_pair.first.x;
  int dy = coordinate_pair.second.y - coordinate_pair.first.y;
  if (dy % dx == 0) {
    int k = dy / dx;
    if (k == 1 || k == -1) {
      return true;
    }
  }
  return false;
}

int FindMax(const vector<pair<Coordinates, Coordinates>>& coordinate_pairs, bool x) {
  int m = numeric_limits<int>::min();
  for (auto& p : coordinate_pairs) {
    if (x) {
      m = max(m, p.first.x);
      m = max(m, p.second.x);
    } else {
      m = max(m, p.first.y);
      m = max(m, p.second.y);
    }
  }
  return m;
}

int Solve() {
  vector<pair<Coordinates, Coordinates>> coordinate_pairs = ParseInput();

  vector<pair<Coordinates, Coordinates>> filtered_pairs;
  copy_if(coordinate_pairs.begin(),
          coordinate_pairs.end(),
          back_inserter(filtered_pairs),
          [](auto& p) { return IsHorizontal(p) || IsVertical(p) || IsDiagonal(p); });

  int x_max = FindMax(filtered_pairs, true);
  int y_max = FindMax(filtered_pairs, false);
  vector<vector<int>> m(y_max + 1, vector<int>(x_max + 1));
  for (auto& p : filtered_pairs) {
    if (IsHorizontal(p)) {
      for (int x = min(p.first.x, p.second.x); x <= max(p.first.x, p.second.x); ++x) {
        ++m[p.first.y][x];
      }
    } else if (IsVertical(p)) {
      for (int y = min(p.first.y, p.second.y); y <= max(p.first.y, p.second.y); ++y) {
        ++m[y][p.first.x];
      }
    } else if (IsDiagonal(p)) {
      auto min_point = min(p.first, p.second);
      auto max_point = max(p.first, p.second);
      int x = min_point.x;
      int y = min_point.y;
      int k = (max_point.y - min_point.y) / (max_point.x - min_point.x);
      while (true) {
        ++m[y][x];
        ++x;
        y += k;
        if (x > max_point.x && (y > max_point.y || y < max_point.y)) {
          break;
        }
      }
    }
  }

  int count = 0;
  for (int i = 0; i < static_cast<int>(m.size()); ++i) {
    for (int j = 0; j < static_cast<int>(m[0].size()); ++j) {
      if (m[i][j] >= 2) {
        ++count;
      }
    }
  }
  return count;
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}
