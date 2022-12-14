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

map<pair<char, char>, char> ParseInsertionRules() {
  map<pair<char, char>, char> insertion_rules;
  string s;
  while (getline(cin, s) && !s.empty()) {
    char a = s[0];
    char b = s[1];
    char c = s[s.size() - 1];
    insertion_rules.emplace(make_pair(a, b), c);
  }
  return insertion_rules;
}

ll Solve() {
  string base;
  cin >> base;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  cin.get();
  map<pair<char, char>, char> insertion_rules = ParseInsertionRules();

  map<pair<char, char>, ll> counts;
  for (int i = 0; i < base.size() - 1; ++i) {
    char a = base[i];
    char b = base[i + 1];
    ++counts[make_pair(a, b)];
  }

  for (int i = 0; i < 40; ++i) {
    map<pair<char, char>, ll> new_counts;
    for (const auto &[pair, count] : counts) {
      const char c = insertion_rules[pair];
      char a = pair.first;
      new_counts[make_pair(a, c)] += count;
      char b = pair.second;
      new_counts[make_pair(c, b)] += count;
    }
    counts = std::move(new_counts);
  }

  unordered_map<char, ll> freq;
  for (const auto &[pair, count] : counts) {
    freq[pair.first] += count;
    freq[pair.second] += count;
  }
  ll minimum = numeric_limits<ll>::max();
  ll maximum = 0;
  for (const auto &[c, f] : freq) {
    minimum = min(minimum, (f + 1) / 2);
    maximum = max(maximum, (f + 1) / 2);
  }

  return maximum - minimum;
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}
