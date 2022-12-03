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

string Compute(const vector<string>& numbers, bool oxygen) {
  vector<string> candidates = numbers;
  // Assuming each number has equivalent amount of bits.
  int bits = static_cast<int>(numbers[0].size());
  for (int bit = 0; bit < bits; ++bit) {
    vector<string> zeros;
    vector<string> ones;
    for (const string& number : candidates) {
      if (number[bit] == '0') {
        zeros.push_back(number);
      } else {
        ones.push_back(number);
      }
    }

    candidates.clear();

    if (oxygen) {
      if (ones.size() >= zeros.size()) {
        for (string& number : ones) {
          candidates.push_back(move(number));
        }
      } else {
        for (string& number : zeros) {
          candidates.push_back(move(number));
        }
      }
    } else {
      if (ones.size() < zeros.size()) {
        for (string& number : ones) {
          candidates.push_back(move(number));
        }
      } else {
        for (string& number : zeros) {
          candidates.push_back(move(number));
        }
      }
    }
    if (candidates.size() == 1) {
      return candidates[0];
    }
  }
  return "";  // Should not reach this ever as long as input is correct.
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
  string oxygen = Compute(numbers, true);
  int oxygen_decimal = BitsToDecimal(oxygen);
  string carbon_dioxide = Compute(numbers, false);
  int carbon_dioxide_decimal = BitsToDecimal(carbon_dioxide);
  return oxygen_decimal * carbon_dioxide_decimal;
}

int main() {
  int multiplication = Solve();
  cout << multiplication << endl;
}
