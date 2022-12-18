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

struct Cube {
  int x = 0, y = 0, z = 0;
  int sides = 6;

  [[nodiscard]] bool IsTouching(const Cube &other) const {
    return Common(other) == 2 && abs(x - other.x) <= 1 &&
           abs(y - other.y) <= 1 && abs(z - other.z) <= 1;
  }

  [[nodiscard]] int Common(const Cube &other) const {
    int common = 0;
    if (x == other.x)
      ++common;
    if (y == other.y)
      ++common;
    if (z == other.z)
      ++common;
    return common;
  }
};

vector<Cube> ReadInput() {
  vector<Cube> cubes;
  string line;
  while (getline(cin, line) && !line.empty()) {
    const auto first_comma_index = line.find(',');
    const auto second_comma_index = line.find(',', first_comma_index + 1);
    const int x = stoi(line.substr(0, first_comma_index));
    const int y = stoi(line.substr(first_comma_index + 1,
                                   second_comma_index - first_comma_index - 1));
    const int z = stoi(line.substr(second_comma_index + 1));
    cubes.push_back({x, y, z});
  }
  return cubes;
}

int Solve() {
  vector<Cube> cubes = ReadInput();
  for (auto &cube : cubes) {
    for (auto &other : cubes) {
      if (cube.IsTouching(other)) {
        --cube.sides;
      }
    }
  }

  int answer = 0;
  for (const auto &cube : cubes) {
    answer += cube.sides;
  }
  return answer;
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}