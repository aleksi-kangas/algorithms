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

vector<int> ReadAndParseInput() {
  vector<int> total_calories(1);
  string line;
  int emptyLines = 0;
  int elf = 0;
  while (getline(cin, line)) {
    if (line.empty()) {
      if (++emptyLines >= 2) {
        break;
      }
      total_calories.emplace_back();
      ++elf;
    } else {
      emptyLines = 0;
      total_calories[elf] += stoi(line);
    }
  }
  return total_calories;
}

int Solve() {
  vector<int> total_calories = ReadAndParseInput();
  return *max_element(total_calories.begin(), total_calories.end());
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}