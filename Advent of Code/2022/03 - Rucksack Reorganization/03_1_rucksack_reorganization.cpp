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

using Compartment = unordered_set<char>;

vector<pair<Compartment, Compartment>> ReadRucksacks() {
  vector<pair<Compartment, Compartment>> rucksacks;
  string r;
  while (getline(cin, r) && !r.empty()) {
    const size_t m = r.size() / 2;
    pair<Compartment, Compartment> compartments{};
    for (size_t i = 0; i < r.size(); ++i) {
      if (i < m) compartments.first.insert(r[i]);
      else compartments.second.insert(r[i]);
    }
    rucksacks.push_back(std::move(compartments));
  }
  return rucksacks;
}

int Solve() {
  const auto rucksacks = ReadRucksacks();
  int priority_sum = 0;
  for (const auto& [compartment1, compartment2] : rucksacks) {
    for (char c = 'a'; c <= 'z'; ++c) {
      if (compartment1.contains(c) && compartment2.contains(c)) {
        priority_sum += c - 'a' + 1;
      }
    }
    for (char c = 'A'; c <= 'Z'; ++c) {
      if (compartment1.contains(c) && compartment2.contains(c)) {
        priority_sum += c - 'A' + 27;
      }
    }
  }
  return priority_sum;
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}