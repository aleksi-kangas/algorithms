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

enum class RPS { Rock, Paper, Scissors };

RPS CharToRPS(char c) {
  switch (c) {
    case 'A': return RPS::Rock;
    case 'B': return RPS::Paper;
    case 'C': return RPS::Scissors;
    case 'X': return RPS::Rock;
    case 'Y': return RPS::Paper;
    case 'Z': return RPS::Scissors;
    default: throw invalid_argument{"Unknown character"};
  }
}

vector<pair<RPS, RPS>> ReadStrategy() {
  vector<pair<RPS, RPS>> strategy;
  string line;
  while (getline(cin, line) && !line.empty()) {
    const char p1 = line[0];
    const char p2 = line[2];
    strategy.emplace_back(CharToRPS(p1), CharToRPS(p2));
  }
  return strategy;
}

int Winner(RPS p1, RPS p2) {
  if (p1 == RPS::Rock && p2 == RPS::Rock) return 0;
  if (p1 == RPS::Paper && p2 == RPS::Paper) return 0;
  if (p1 == RPS::Scissors && p2 == RPS::Scissors) return 0;

  if (p1 == RPS::Rock && p2 == RPS::Paper) return 1;
  if (p1 == RPS::Rock && p2 == RPS::Scissors) return -1;

  if (p1 == RPS::Paper && p2 == RPS::Rock) return -1;
  if (p1 == RPS::Paper && p2 == RPS::Scissors) return 1;

  if (p1 == RPS::Scissors && p2 == RPS::Rock) return 1;
  if (p1 == RPS::Scissors && p2 == RPS::Paper) return -1;

  return 0;
}

int Solve() {
  const vector<pair<RPS, RPS>> strategy = ReadStrategy();

  int total_score = 0;
  for (const auto& [p1, p2] : strategy) {
    total_score += p2 == RPS::Rock ? 1 : p2 == RPS::Paper ? 2 : p2 == RPS::Scissors ? 3 : 0;
    const int winner = Winner(p1, p2);
    if (winner == 0) total_score += 3;
    if (winner > 0) total_score += 6;
  }

  return total_score;
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}