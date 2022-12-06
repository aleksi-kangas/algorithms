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

int Solve() {
  string line;
  getline(cin, line);

  for (int i = 3; i < line.size(); ++i) {
    unordered_set<char> seen;
    for (int j = 0; j < 4; ++j) {
      seen.insert(line[i - j]);
    }
    if (seen.size() == 4) {
      return i + 1;
    }
  }
  return -1;
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}