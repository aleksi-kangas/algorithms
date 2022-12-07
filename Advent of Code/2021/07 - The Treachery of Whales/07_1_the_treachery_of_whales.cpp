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

vector<int> ParseInput() {
  vector<int> positions;
  string s;
  cin >> s;
  stringstream ss(s);
  for (int i; ss >> i;) {
    positions.push_back(i);
    if (ss.peek() == ',') {
      ss.ignore();
    }
  }
  return positions;
}

int Solve() {
  vector<int> positions = ParseInput();
  sort(positions.begin(), positions.end());
  int n = static_cast<int>(positions.size());
  int target = positions[n / 2];
  int fuel = 0;
  for (int position : positions) {
    fuel += abs(target - position);
  }
  return fuel;
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}
