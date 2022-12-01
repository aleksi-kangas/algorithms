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
  string current, previous;
  while (getline(cin, current) && !current.empty()) {
    if (previous.empty()) {
      previous = current;
      continue;
    }

    int a = stoi(previous);
    int b = stoi(current);
    if (b > a) {
      ++answer;
    }
    previous = current;
  }
  return answer;
}

int main() {
  int count = Solve();
  cout << count << endl;
}
