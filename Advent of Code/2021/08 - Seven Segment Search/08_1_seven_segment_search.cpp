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
  int answer = 0;
  string line;
  while (getline(cin, line) && !line.empty()) {
    stringstream ss(line);
    string s;
    bool delimiter_seen = false;
    while (ss >> s) {
      if (s == "|") {
        delimiter_seen = true;
        continue;
      }
      if (delimiter_seen) {
        if (s.size() == 2 || s.size() == 3 || s.size() == 4 || s.size() == 7) {
          ++answer;
        }
      }
    }
  }
  return answer;
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}
