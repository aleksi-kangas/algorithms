#include <algorithm>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iomanip>
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
#include <variant>
#include <vector>

using namespace std;

using ll = long long;

vector<int> ParseFormat(const string& format) {
  vector<int> partitions{};
  stringstream ss{format};
  std::string partition{};
  while (getline(ss, partition, '-'))
    partitions.push_back(stoi(partition));
  return partitions;
}

string DigitToString(int digit) {
  switch (digit) {
    case 0:
      return "zero";
    case 1:
      return "one";
    case 2:
      return "two";
    case 3:
      return "three";
    case 4:
      return "four";
    case 5:
      return "five";
    case 6:
      return "six";
    case 7:
      return "seven";
    case 8:
      return "eight";
    case 9:
      return "nine";
    default:
      throw runtime_error{"Unknown digit: " + to_string(digit)};
  }
}

string Generate(char c, int count) {
  string digit = DigitToString(c - '0');
  if (count == 1)
    return digit;
  else if (count == 2)
    return "double " + digit;
  else if (count == 3)
    return "triple " + digit;
  else if (count == 4)
    return "quadruple " + digit;
  else if (count == 5)
    return "quintuple " + digit;
  else if (count == 6)
    return "sextuple " + digit;
  else if (count == 7)
    return "septuple " + digit;
  else if (count == 8)
    return "octuple " + digit;
  else if (count == 9)
    return "nonuple " + digit;
  else if (count == 10)
    return "decuple " + digit;
  else if (count > 10) {
    stringstream ss{};
    for (int i = 1; i < count; ++i)
      ss << digit << ' ';
    ss << digit;
    return ss.str();
  } else
    throw runtime_error{"Unexpected"};
}

vector<string> ProcessPartition(const string& number, pair<int, int> range) {
  vector<string> result{};
  int i = range.first;
  while (i <= range.second) {
    int j = i;
    while (j <= range.second && number[i] == number[j])
      ++j;
    const int count = j - i;
    result.push_back(Generate(number[i], count));
    i = j;
  }
  return result;
}

string Solve() {
  string number;
  cin >> number;
  string format;
  cin >> format;
  const vector<int> partitions = ParseFormat(format);
  stringstream ss{};
  int i = 0;
  for (int p : partitions) {
    for (const string& s : ProcessPartition(number, {i, i + p - 1})) {
      ss << s << ' ';
    }
    i = i + p;
  }
  return ss.str();
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int test_cases;
  cin >> test_cases;

  for (int test_case = 1; test_case <= test_cases; ++test_case) {
    auto answer = Solve();
    cout << "Case #" << test_case << ": " << answer << '\n';
  }
}
