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

ll Solve() {
  vector<string> lines = ParseInput();
  vector<ll> scores;
  for (const auto& line : lines) {
    ll score = 0;
    stack<char> s;
    bool corrupt = false;
    for (char c : line) {
      bool is_opening = c == '(' || c == '[' || c == '{' || c == '<';
      if (is_opening) s.push(c);
      else {
        if (s.empty()) {
          corrupt = true;
          break;
        }
        char opening = s.top();
        bool is_matching = (c == ')' && opening == '(') ||
            (c == ']' && opening == '[') ||
            (c == '}' && opening == '{') ||
            (c == '>' && opening == '<');
        if (is_matching) s.pop();
        else {
          corrupt = true;
          break;
        }
      }
    }
    if (!corrupt) {
      while (!s.empty()) {
        char opening = s.top();
        s.pop();
        score *= 5;
        if (opening == '(') score += 1;
        else if (opening == '[') score += 2;
        else if (opening == '{') score += 3;
        else if (opening == '<') score += 4;
      }
      scores.push_back(score);
    }
  }

  sort(scores.begin(), scores.end());
  return scores[scores.size() / 2];
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}
