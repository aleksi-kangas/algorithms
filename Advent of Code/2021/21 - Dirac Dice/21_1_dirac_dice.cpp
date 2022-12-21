#include <algorithm>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iomanip>
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
#include <variant>
#include <vector>

using namespace std;

using ll = long long;

pair<int, int> ParseStartingPositions() {
  string s;
  getline(cin, s);
  int p1 = stoi(s.substr(s.find(':') + 1));
  getline(cin, s);
  int p2 = stoi(s.substr(s.find(':') + 1));
  return {p1, p2};
}

int PlayTurn(int& dice) {
  int s = 0;
  for (int i = 0; i < 3; ++i) {
    s += dice++;
    if (dice == 101) dice = 1;
  }
  return s;
}

int Solve() {
  auto [p1, p2] = ParseStartingPositions();
  int round = 0;
  int dice = 1;
  int p1_score = 0, p2_score = 0;
  bool p1_plays = true;
  while (p1_score < 1000 && p2_score < 1000) {
    ++round;
    if (p1_plays) {
      p1 += PlayTurn(dice);
      p1 %= 10;
      if (p1 == 0) p1 = 10;
      p1_score += p1;
      p1_plays = false;
    } else {
      p2 += PlayTurn(dice);
      p2 %= 10;
      if (p2 == 0) p2 = 10;
      p2_score += p2;
      p1_plays = true;
    }
  }
  return (p1_score > p2_score ? p2_score : p1_score) * round * 3;
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}
