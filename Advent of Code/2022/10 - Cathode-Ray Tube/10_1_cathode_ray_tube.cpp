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

enum class InstructionType { kNoOp, kAddX };

struct Instruction {
  InstructionType type = InstructionType::kNoOp;
  int value = 0;
  int cycles = 1;
  int start = 1;
};

struct CompareInstruction {
  bool operator()(const Instruction &lhs, const Instruction &rhs) const {
    return lhs.start > rhs.start;  // For min heap
  }
};

vector<Instruction> ReadInput() {
  vector<Instruction> instructions;
  string line;
  int start = 0;
  while (getline(cin, line) && !line.empty()) {
    stringstream ss{line};
    vector<string> tokens;
    while (getline(ss, line, ' ')) {
      tokens.push_back(line);
    }
    if (tokens.size() == 1 && tokens[0] == "noop") {
      instructions.emplace_back(
          Instruction{InstructionType::kNoOp, 0, 1, start});
      start += instructions.back().cycles;
    } else if (tokens[0] == "addx") {
      instructions.emplace_back(
          Instruction{InstructionType::kAddX, stoi(tokens[1]), 2, start});
      start += instructions.back().cycles;
    } else {
      assert(false);
    }
  }
  return instructions;
}

int Solve() {
  auto instructions = ReadInput();

  priority_queue<Instruction, vector<Instruction>, CompareInstruction> pq;
  for (auto &instruction : instructions) {
    pq.push(instruction);
  }

  unordered_set<int> seen_cycles;
  int signal_strength = 0;
  int cycles = 0;
  int register_x = 1;
  while (!pq.empty()) {
    const auto instruction = pq.top();
    pq.pop();
    cycles += instruction.cycles;
    if (cycles >= 20 && !seen_cycles.contains(20)) {
      signal_strength += register_x * 20;
      seen_cycles.insert(20);
    }
    if (cycles >= 60 && !seen_cycles.contains(60)) {
      signal_strength += register_x * 60;
      seen_cycles.insert(60);
    }
    if (cycles >= 100 && !seen_cycles.contains(100)) {
      signal_strength += register_x * 100;
      seen_cycles.insert(100);
    }
    if (cycles >= 140 && !seen_cycles.contains(140)) {
      signal_strength += register_x * 140;
      seen_cycles.insert(140);
    }
    if (cycles >= 180 && !seen_cycles.contains(180)) {
      signal_strength += register_x * 180;
      seen_cycles.insert(180);
    }
    if (cycles >= 220 && !seen_cycles.contains(220)) {
      signal_strength += register_x * 220;
      seen_cycles.insert(220);
    }
    switch (instruction.type) {
    case InstructionType::kNoOp:
      break;
    case InstructionType::kAddX:
      register_x += instruction.value;
      break;
    default:
      assert(false);
    }
  }

  return signal_strength;
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}