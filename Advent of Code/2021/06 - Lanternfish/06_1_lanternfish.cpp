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

struct Fish {
  Fish() : timer(8) {}
  explicit Fish(int timer) : timer(timer) {}
  int timer;
};

vector<Fish> ParseInput() {
  string s;
  cin >> s;
  vector<Fish> fish;
  stringstream ss(s);
  for (int i; ss >> i;) {
    fish.emplace_back(i);
    if (ss.peek() == ',') {
      ss.ignore();
    }
  }
  return fish;
}

int Solve() {
  vector<Fish> fish = ParseInput();
  for (int day = 1; day <= 80; ++day) {
    vector<Fish> new_fish;
    for (auto& f : fish) {
      if (f.timer == 0) {
        f.timer = 6;
        new_fish.emplace_back();
      } else {
        --f.timer;
      }
    }
    fish.insert(fish.end(), new_fish.begin(), new_fish.end());
  }
  return static_cast<int>(fish.size());
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}
