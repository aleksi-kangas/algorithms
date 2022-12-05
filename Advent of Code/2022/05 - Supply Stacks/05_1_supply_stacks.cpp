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

vector<deque<char>> ReadStacks() {
  vector<deque<char>> stacks;
  string line;
  constexpr size_t kRawStackLength = 3;
  while (getline(cin, line) && !line.empty()) {
    const size_t kStacksPerRow = line.size() / (kRawStackLength + 1) + 1;
    if (stacks.size() <  kStacksPerRow) {
      stacks.resize(kStacksPerRow);
    }
    for (size_t i = 0; i < kStacksPerRow; ++i) {
      char c = line[i * (kRawStackLength + 1) + 1];
      if (isalpha(c)) {
        stacks[i].push_back(c);
      }
    }
  }
  return stacks;
}

struct Instruction {
  int count = 0;
  pair<int, int> from_to{};
};

vector<Instruction> ReadInstructions() {
  vector<Instruction> instructions;
  string line;
  while (getline(cin, line) && !line.empty()) {
    Instruction instruction;
    istringstream iss(line);
    iss.ignore(5);
    iss >> instruction.count;
    iss.ignore(6);
    iss >> instruction.from_to.first;
    --instruction.from_to.first;
    iss.ignore(4);
    iss >> instruction.from_to.second;
    --instruction.from_to.second;
    instructions.push_back(std::move(instruction));
  }
  return instructions;
}

string Solve() {
  auto stacks = ReadStacks();
  const auto instructions = ReadInstructions();

  for (const auto &instruction : instructions) {
    auto &from = stacks[instruction.from_to.first];
    auto &to = stacks[instruction.from_to.second];
    for (int i = 0; i < instruction.count; ++i) {
      to.push_front(from.front());
      from.pop_front();
    }
  }

  stringstream ss;
  for (const auto &stack : stacks) {
    if (stack.empty())
      continue;
    ss << stack.front();
  }
  return ss.str();
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}