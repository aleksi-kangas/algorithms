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

enum class Direction { X, Y };

vector<pair<int, int>> ReadDots() {
  vector<pair<int, int>> dots;
  string s;
  while (getline(cin, s) && !s.empty()) {
    auto comma = s.find(',');
    int a = stoi(s.substr(0, comma));
    int b = stoi(s.substr(comma + 1));
    dots.emplace_back(a, b);
  }
  return dots;
}

vector<pair<Direction, int>> ReadFolds() {
  vector<pair<Direction, int>> folds;
  string s;
  while (getline(cin, s) && !s.empty()) {
    auto equals = s.find('=');
    Direction direction = s[equals - 1] == 'x' ? Direction::X : Direction::Y;
    int value = stoi(s.substr(equals + 1));
    folds.emplace_back(direction, value);
  }
  return folds;
}

void Fold(const pair<Direction, int> &fold, vector<pair<int, int>> &dots) {
  for (auto &[x, y] : dots) {
    switch (fold.first) {
      case Direction::X:
        if (x > fold.second) {
          x = fold.second - (x - fold.second);
        }
        break;
      case Direction::Y:
        if (y > fold.second) {
          y = fold.second - (y - fold.second);
        }
        break;
    }
  }
}

pair<int, int> FindMax(const vector<pair<int, int>> &dots) {
  int x = numeric_limits<int>::min();
  int y = numeric_limits<int>::min();
  for (const auto &d : dots) {
    x = max(x, d.first);
    y = max(y, d.second);
  }
  return {x, y};
}

int Solve() {
  vector<pair<int, int>> dots = ReadDots();
  vector<pair<Direction, int>> folds = ReadFolds();

  Fold(folds.front(), dots);

  auto[x_max, y_max] = FindMax(dots);
  vector<vector<int>> a(y_max + 1, vector<int>(x_max + 1));
  for (const auto &[x, y] : dots) {
    a[y][x] = 1;
  }
  int count = 0;
  for (int i = 0; i <= y_max; ++i) {
    for (int j = 0; j <= x_max; ++j) {
      if (a[i][j] == 1) {
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
