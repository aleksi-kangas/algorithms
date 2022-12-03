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

vector<string> ReadRucksacks() {
  vector<string> rucksacks;
  string r;
  while (getline(cin, r) && !r.empty()) {
    rucksacks.push_back(std::move(r));
  }
  return rucksacks;
}

int Solve() {
  const auto rucksacks = ReadRucksacks();
  int priority_sum = 0;
  for (size_t i = 0; i < rucksacks.size(); i += 3) {
    unordered_map<char, int> counts;
    for (size_t j = 0; j < 3; ++j) {
      unordered_set<char> seen;
      for (char c : rucksacks[i + j]) {
        seen.insert(c);
      }
      for (char c : seen) {
        ++counts[c];
      }
    }
    for (auto& [c, count] : counts) {
      if (count != 3) continue;
      if (islower(c)) priority_sum += c - 'a' + 1;
      else priority_sum += c - 'A' + 27;
    }
  }
  return priority_sum;
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}