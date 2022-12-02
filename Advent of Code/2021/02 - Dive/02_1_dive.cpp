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

enum class Direction { Forward, Down, Up };

struct Command {
  Direction direction = Direction::Forward;
  int value = 0;
};

optional<Command> ParseCommand(const string& s) {
  auto split = s.find(' ');
  if (split == string::npos) {
    return nullopt;
  }
  Command command;
  string direction = s.substr(0, split);
  if (direction == "down") {
    command.direction = Direction::Down;
  } else if (direction == "up") {
    command.direction = Direction::Up;
  }
  string value = s.substr(split);
  command.value = stoi(value);
  return command;
}

int Solve() {
  int depth = 0;
  int x = 0;
  string s;
  while (getline(cin, s) && !s.empty()) {
    optional<Command> command = ParseCommand(s);
    if (!command.has_value()) {
      return x * depth;
    }
    switch (command->direction) {
      case Direction::Forward:
        x += command->value;
        break;
      case Direction::Down:
        depth += command->value;
        break;
      case Direction::Up:
        depth -= command->value;
        break;
    }
  }
  return x * depth;
}

int main() {
  int multiplication = Solve();
  cout << multiplication << endl;
}
