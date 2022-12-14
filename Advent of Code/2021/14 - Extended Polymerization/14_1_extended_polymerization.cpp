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

struct Node {
  char c;
  Node* next;
};

map<pair<char, char>, char> ParseInsertionRules() {
  map<pair<char, char>, char> insertion_rules;
  string s;
  while (getline(cin, s) && !s.empty()) {
    char a = s[0];
    char b = s[1];
    char c = s[s.size() - 1];
    insertion_rules.emplace(make_pair(a, b), c);
  }
  return insertion_rules;
}

int Solve() {
  string base;
  cin >> base;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  cin.get();
  map<pair<char, char>, char> insertion_rules = ParseInsertionRules();

  Node* head = nullptr;
  Node* previous = nullptr;
  for (char c : base) {
    Node* node = new Node{c, nullptr};
    if (head == nullptr) head = node;
    if (previous != nullptr) previous->next = node;
    previous = node;
  }

  for (int i = 0; i < 10; ++i) {
    Node* current = head;
    while (current != nullptr && current->next != nullptr) {
      const char a = current->c;
      Node* next = current->next;
      const char b = next->c;
      const char c = insertion_rules[make_pair(a, b)];
      Node* between = new Node{c, nullptr};
      current->next = between;
      between->next = next;
      current = next;
    }
  }

  unordered_map<char, int> freq;
  Node* current = head;
  while (current != nullptr) {
    ++freq[current->c];
    current = current->next;
  }
  int maximum = 0;
  int minimum = numeric_limits<int>::max();
  for (const auto& [c, f] : freq) {
    maximum = max(maximum, f);
    minimum = min(minimum, f);
  }

  return maximum - minimum;
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}
