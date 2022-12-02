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
enum class Winner { P1, Draw, P2 };

RPS CharToRPS(char c) {
  switch (c) {
    case 'A': return RPS::Rock;
    case 'B': return RPS::Paper;
    case 'C': return RPS::Scissors;
    default: throw invalid_argument{"Unknown character"};
  }
}

Winner CharToWinner(char c) {
  switch (c) {
    case 'X': return Winner::P1;
    case 'Y': return Winner::Draw;
    case 'Z': return Winner::P2;
    default: throw invalid_argument{"Unknown character"};
  }
}

vector<pair<RPS, Winner>> ReadStrategy() {
  vector<pair<RPS, Winner>> strategy;
  string line;
  while (getline(cin, line) && !line.empty()) {
    const char p1 = line[0];
    const char winner = line[2];
    strategy.emplace_back(CharToRPS(p1), CharToWinner(winner));
  }
  return strategy;
}

int Score(RPS rps) {
  switch (rps) {
    case RPS::Rock: return 1;
    case RPS::Paper: return 2;
    case RPS::Scissors: return 3;
  }
}

int Solve() {
  const vector<pair<RPS, Winner>> strategy = ReadStrategy();

  int total_score = 0;
  for (const auto &[p1, winner] : strategy) {
    switch (winner) {
      case Winner::Draw: {
        total_score += 3;
        total_score += Score(p1);
      } break;
      case Winner::P1: {
        if (p1 == RPS::Rock) total_score += Score(RPS::Scissors);
        if (p1 == RPS::Paper) total_score += Score(RPS::Rock);
        if (p1 == RPS::Scissors) total_score += Score(RPS::Paper);
      } break;
      case Winner::P2: {
        total_score += 6;
        if (p1 == RPS::Rock) total_score += Score(RPS::Paper);
        if (p1 == RPS::Paper) total_score += Score(RPS::Scissors);
        if (p1 == RPS::Scissors) total_score += Score(RPS::Rock);
      } break;
    }
  }

  return total_score;
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}