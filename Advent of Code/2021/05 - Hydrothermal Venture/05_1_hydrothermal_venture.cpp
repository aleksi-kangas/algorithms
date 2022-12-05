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
  vector<pair<Coordinates, Coordinates>> straight_lines;
  copy_if(coordinate_pairs.begin(),
          coordinate_pairs.end(),
          back_inserter(straight_lines),
          [](auto& p) { return p.first.x == p.second.x || p.first.y == p.second.y; });
  int x_max = FindMax(straight_lines, true);
  int y_max = FindMax(straight_lines, false);
  vector<vector<int>> m(y_max + 1, vector<int>(x_max + 1));
  for (auto& p : straight_lines) {
    if (p.first.x != p.second.x) {
      for (int i = min(p.first.x, p.second.x); i <= max(p.first.x, p.second.x); ++i) {
        ++m[p.first.y][i];
      }
    } else {
      for (int i = min(p.first.y, p.second.y); i <= max(p.first.y, p.second.y); ++i) {
        ++m[i][p.first.x];
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
