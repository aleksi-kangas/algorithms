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

using Range = pair<int, int>;

vector<pair<Range, Range>> ReadInput() {
  vector<pair<Range, Range>> pairs;
  string line;
  while (getline(cin, line) && !line.empty()) {
    auto comma_index = line.find(',');
    auto first = line.substr(0, comma_index);
    auto second = line.substr(comma_index + 1);
    auto first_dash_index = first.find('-');
    auto second_dash_index = second.find('-');
    pairs.emplace_back(make_pair(stoi(first.substr(0, first_dash_index)),
                                 stoi(first.substr(first_dash_index + 1))),
                       make_pair(stoi(second.substr(0, second_dash_index)),
                                 stoi(second.substr(second_dash_index + 1))));
  }
  return pairs;
}

int Solve() {
  const vector<pair<Range, Range>> pairs = ReadInput();
  int answer = 0;
  for (const auto &[first, second] : pairs) {
    if (first.first <= second.first && second.second <= first.second) {
      ++answer;
    } else if (second.first <= first.first && first.second <= second.second) {
      ++answer;
    }
  }
  return answer;
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}