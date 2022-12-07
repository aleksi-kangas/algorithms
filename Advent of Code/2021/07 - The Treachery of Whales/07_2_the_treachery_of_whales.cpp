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

ll Solve() {
  vector<int> positions = ParseInput();
  int min_pos = numeric_limits<int>::max();
  int max_pos = numeric_limits<int>::min();
  for (int x : positions) {
    min_pos = min(min_pos, x);
    max_pos = max(max_pos, x);
  }

  int answer = numeric_limits<int>::max();
  for (int pos = min_pos; pos <= max_pos; ++pos) {
    int fuel = 0;
    for (int x : positions) {
      int s = abs(x - pos);
      fuel += s * (s + 1) / 2;
    }
    answer = min(answer, fuel);
  }
  return answer;
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}
