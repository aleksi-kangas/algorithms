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

enum class Tile { kEmpty, kRock, kSand };

vector<pair<int, int>> ParseLine(const string &line) {
  vector<pair<int, int>> result;
  stringstream ss{line};
  vector<string> tokens;
  string token;
  while (ss >> token) {
    if (token != "->") {
      tokens.push_back(token);
    }
  }
  for (const auto &t : tokens) {
    const auto comma_index = t.find(',');
    const auto x = stoi(t.substr(0, comma_index));
    const auto y = stoi(t.substr(comma_index + 1));
    result.emplace_back(x, y);
  }
  return result;
}

set<pair<int, int>> ReadInput() {
  set<pair<int, int>> result;

  string line;
  while (getline(cin, line) && !line.empty()) {
    vector<pair<int, int>> points = ParseLine(line);
    for (size_t i = 1; i <= points.size() - 1; ++i) {
      const auto [x1, y1] = points[i - 1];
      const auto [x2, y2] = points[i];
      for (int x = min(x1, x2); x <= max(x1, x2); ++x) {
        for (int y = min(y1, y2); y <= max(y1, y2); ++y) {
          result.insert({x, y});
        }
      }
    }
  }

  return result;
}

bool SimulateSand(vector<vector<Tile>> &grid) {
  size_t x = 500, y = 0;
  while (true) {
    if (grid[0][500] == Tile::kSand) {
      return false;
    }
    if (grid[y + 1][x] == Tile::kEmpty) {
      ++y;
    } else if (grid[y + 1][x - 1] == Tile::kEmpty) {
      ++y;
      --x;
    } else if (grid[y + 1][x + 1] == Tile::kEmpty) {
      ++y;
      ++x;
    } else {
      grid[y][x] = Tile::kSand;
      return true;
    }
  }
}

int Solve() {
  auto rock_positions = ReadInput();
  int max_y = 0;
  for (const auto &[x, y] : rock_positions) {
    max_y = max(max_y, y);
  }

  vector<vector<Tile>> grid(max_y + 3, vector<Tile>(500 + 500, Tile::kEmpty));
  for (const auto &[x, y] : rock_positions) {
    grid[y][x] = Tile::kRock;
  }

  for (size_t x = 0; x < grid[0].size(); ++x) {
    grid[max_y + 2][x] = Tile::kRock;
  }

  int answer = 0;
  while (true) {
    if (!SimulateSand(grid)) {
      break;
    }
    ++answer;
  }
  return answer;
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}