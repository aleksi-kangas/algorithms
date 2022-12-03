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

string Compute(const vector<string>& numbers, bool gamma) {
  // Assuming each number has equivalent amount of bits.
  int bits = static_cast<int>(numbers[0].size());
  string result;
  for (int bit = 0; bit < bits; ++bit) {
    int zeros = 0;
    int ones = 0;
    for (const string& number : numbers) {
      if (number[bit] == '0') {
        ++zeros;
      } else {
        ++ones;
      }
    }
    if (gamma) {
      if (ones > zeros) {
        result.push_back('1');
      } else {
        result.push_back('0');
      }
    } else {
      if (ones > zeros) {
        result.push_back('0');
      } else {
        result.push_back('1');
      }
    }
  }
  return result;
}

vector<string> ParseInput() {
  vector<string> numbers;
  string s;
  while (getline(cin, s) && !s.empty()) {
    numbers.push_back(s);
  }
  return numbers;
}

int BitsToDecimal(const string& bits) {
  int result = 0;
  for (char bit : bits) {
    result <<= 1;
    result |= bit - '0';
  }
  return result;
}

int Solve() {
  vector<string> numbers = ParseInput();
  string gamma = Compute(numbers, true);
  int gamma_decimal = BitsToDecimal(gamma);
  string epsilon = Compute(numbers, false);
  int epsilon_decimal = BitsToDecimal(epsilon);
  return gamma_decimal * epsilon_decimal;
}

int main() {
  int multiplication = Solve();
  cout << multiplication << endl;
}
