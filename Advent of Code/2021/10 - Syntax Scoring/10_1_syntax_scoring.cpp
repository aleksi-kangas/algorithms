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

vector<string> ParseInput() {
  vector<string> lines;
  string s;
  while (getline(cin, s) && !s.empty()) {
    lines.push_back(move(s));
  }
  return lines;
}

int Solve() {
  vector<string> lines = ParseInput();
  int answer = 0;
  for (const auto& line : lines) {
    stack<char> s;
    for (char c : line) {
      bool is_opening = c == '(' || c == '[' || c == '{' || c == '<';
      if (is_opening) s.push(c);
      else {
        if (s.empty()) {
          if (c == ')') answer += 3;
          else if (c == ']') answer += 57;
          else if (c == '}') answer += 1197;
          else if (c == '>') answer += 25137;
          break;
        }
        char opening = s.top();
        bool is_matching = (c == ')' && opening == '(') ||
            (c == ']' && opening == '[') ||
            (c == '}' && opening == '{') ||
            (c == '>' && opening == '<');
        if (is_matching) s.pop();
        else {
          if (c == ')') answer += 3;
          else if (c == ']') answer += 57;
          else if (c == '}') answer += 1197;
          else if (c == '>') answer += 25137;
          break;
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
