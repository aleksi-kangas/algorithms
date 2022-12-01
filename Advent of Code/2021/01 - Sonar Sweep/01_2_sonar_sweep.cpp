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

vector<int> ReadDepths() {
  vector<int> depths;
  string s;
  while (getline(cin, s) && !s.empty()) {
    int a = stoi(s);
    depths.push_back(a);
  }
  return depths;
}

int Solve() {
  vector<int> depths = ReadDepths();
  int answer = 0;
  constexpr int kWindowSize = 3;
  for (int i = 0; i < static_cast<int>(depths.size()) - kWindowSize; ++i) {
    int a = 0;
    int b = 0;
    for (int j = i; j < i + kWindowSize; ++j) {
      a += depths[j];
      b += depths[j + 1];
    }
    if (b > a) {
      ++answer;
    }
  }
  return answer;
}

int main() {
  int count = Solve();
  cout << count << endl;
}
