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

enum class Direction { kLeft, kRight, kUp, kDown };

vector<pair<Direction, int>> ReadInput() {
  vector<pair<Direction, int>> instructions;
  string line;
  while (getline(cin, line) && !line.empty()) {
    char dir = line[0];
    int dist = stoi(line.substr(1));
    switch (dir) {
    case 'L':
      instructions.emplace_back(Direction::kLeft, dist);
      break;
    case 'R':
      instructions.emplace_back(Direction::kRight, dist);
      break;
    case 'U':
      instructions.emplace_back(Direction::kUp, dist);
      break;
    case 'D':
      instructions.emplace_back(Direction::kDown, dist);
      break;
    default:
      assert(false);
    }
  }
  return instructions;
}

void MoveHead(pair<int, int> &head, Direction direction) {
  switch (direction) {
  case Direction::kLeft:
    --head.first;
    break;
  case Direction::kRight:
    ++head.first;
    break;
  case Direction::kUp:
    ++head.second;
    break;
  case Direction::kDown:
    --head.second;
    break;
  default:
    assert(false);
  }
}

void MoveTail(const pair<int, int> &head, pair<int, int> &tail) {
  int dx = head.first - tail.first;
  int dy = head.second - tail.second;
  if (dx == 0 || dy == 0) {
    if (abs(dx) >= 2) {
      tail.first += (dx > 0) ? 1 : -1;
    }
    if (abs(dy) >= 2) {
      tail.second += (dy > 0) ? 1 : -1;
    }
  } else if (abs(dx) != 1 || abs(dy) != 1) {
    tail.first += (dx > 0) ? 1 : -1;
    tail.second += (dy > 0) ? 1 : -1;
  }
}

int Solve() {
  vector<pair<Direction, int>> instructions = ReadInput();
  pair<int, int> head = {0, 0};
  vector<pair<int, int>> knots(9, {0, 0});
  set<pair<int, int>> visited;
  visited.insert(knots[8]);

  for (const auto &[direction, distance] : instructions) {
    for (int d = 0; d < distance; ++d) {
      // Move the head
      MoveHead(head, direction);

      // Move the knots
      pair<int, int> previous = head;
      for (int k = 0; k < 9; ++k) {
        MoveTail(previous, knots[k]);
        previous = knots[k];
      }
      visited.insert(knots[8]);
    }
  }

  return static_cast<int>(visited.size());
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}