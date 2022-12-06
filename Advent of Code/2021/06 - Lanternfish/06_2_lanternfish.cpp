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

ll Solve() {
  vector<Fish> fish = ParseInput();
  unordered_map<int, ll> fish_timers;
  for (int i = 0; i < 9; ++i) {
    fish_timers[i] = 0;
  }
  for (const auto& f : fish) {
    ++fish_timers[f.timer];
  }
  for (int day = 1; day <= 256; ++day) {
    const ll new_fish = fish_timers[0];
    for (int i = 0; i < 8; ++i) {
      fish_timers[i] = fish_timers[i + 1];
    }
    fish_timers[6] = fish_timers[6] + new_fish;
    fish_timers[8] = new_fish;
  }
  ll sum = 0;
  for (int i = 0; i < 9; ++i) {
    sum += fish_timers[i];
  }
  return sum;
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}
