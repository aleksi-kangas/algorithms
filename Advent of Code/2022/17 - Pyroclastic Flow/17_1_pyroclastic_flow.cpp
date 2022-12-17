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

enum class Direction { kLeft, kRight };
enum class RockType {
  kHorizontal = 0,
  kPlus = 1,
  kInverseL = 2,
  kVertical = 3,
  kSquare = 4
};

vector<pair<int, int>> RockPositions(const pair<int, int> &position,
                                     RockType rock_type) {
  switch (rock_type) {
  case RockType::kHorizontal: // Base second left
    return {{position.first, position.second},
            {position.first - 1, position.second},
            {position.first + 1, position.second},
            {position.first + 2, position.second}};
  case RockType::kPlus: // Base bottom middle
    return {{position.first, position.second},
            {position.first, position.second + 1},
            {position.first - 1, position.second + 1},
            {position.first + 1, position.second + 1},
            {position.first, position.second + 2}};
  case RockType::kInverseL: // Base bottom middle
    return {{position.first, position.second},
            {position.first - 1, position.second},
            {position.first + 1, position.second},
            {position.first + 1, position.second + 1},
            {position.first + 1, position.second + 2}};
  case RockType::kVertical: // Base bottom - 1 left
    return {{position.first - 1, position.second},
            {position.first - 1, position.second + 1},
            {position.first - 1, position.second + 2},
            {position.first - 1, position.second + 3}};
  case RockType::kSquare: // Base bottom right
    return {{position.first, position.second},
            {position.first - 1, position.second},
            {position.first, position.second + 1},
            {position.first - 1, position.second + 1}};
  }
}

struct Chamber {
  explicit Chamber(const queue<Direction> &directions)
      : directions{directions} {
    for (size_t i = 0; i < 7; ++i) {
      occupied.insert({i, 0});
    }
  }

  int Peak() {
    int max_y = 0;
    for (const auto &[_, y] : occupied) {
      max_y = max(max_y, y);
    }
    return max_y;
  }

  void SpawnRock(RockType rock_type) {
    pair<int, int> rock = {7 / 2, Peak() + 3 + 1};
    auto rock_positions = RockPositions(rock, rock_type);
    while (true) {
      // Left/Right
      const auto direction = directions.front();
      directions.pop();
      directions.push(direction);
      switch (direction) {
      case Direction::kLeft: {
        if (std::all_of(rock_positions.begin(), rock_positions.end(),
                        [this](const auto &position) {
                          const pair<int, int> new_position = {
                              position.first - 1, position.second};
                          return new_position.first >= 0 &&
                                 !occupied.contains(new_position);
                        })) {
          for (auto &[x, _] : rock_positions) {
            --x;
          }
        }
      } break;
      case Direction::kRight: {
        if (std::all_of(rock_positions.begin(), rock_positions.end(),
                        [this](const auto &position) {
                          const pair<int, int> new_position = {
                              position.first + 1, position.second};
                          return new_position.first < 7 &&
                                 !occupied.contains(new_position);
                        })) {
          for (auto &[x, _] : rock_positions) {
            ++x;
          }
        }
      } break;
      }
      // Down
      if (std::all_of(rock_positions.begin(), rock_positions.end(),
                      [this](const auto &position) {
                        const pair<int, int> new_position = {
                            position.first, position.second - 1};
                        return new_position.second > 0 &&
                               !occupied.contains(new_position);
                      })) {
        for (auto &[_, y] : rock_positions) {
          --y;
        }
      } else {
        for (const auto &[x, y] : rock_positions) {
          occupied.insert({x, y});
        }
        break;
      }
    }
  }

  int RunSimulation() {
    int rock_count = 0;
    while (rock_count < 2022) {
      SpawnRock(static_cast<RockType>(rock_index % 5));
      ++rock_index;
      ++rock_count;
    }
    return Peak();
  }

  queue<Direction> directions;
  set<pair<int, int>> occupied;
  int rock_index = 0;
};

queue<Direction> ReadInput() {
  queue<Direction> directions;
  string line;
  getline(cin, line);
  for (char c : line) {
    switch (c) {
    case '<':
      directions.push(Direction::kLeft);
      break;
    case '>':
      directions.push(Direction::kRight);
      break;
    default:
      assert(false);
    }
  }
  return directions;
}

int Solve() {
  auto directions = ReadInput();
  Chamber chamber{directions};
  return chamber.RunSimulation();
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}