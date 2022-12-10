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
    return lhs.start > rhs.start; // For min heap
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

void Solve() {
  auto instructions = ReadInput();

  priority_queue<Instruction, vector<Instruction>, CompareInstruction> pq;
  for (auto &instruction : instructions) {
    pq.push(instruction);
  }

  vector<vector<char>> crt(6, vector<char>(40, '.'));

  unordered_set<int> seen_cycles;
  int signal_strength = 0;
  int register_x = 1;
  while (!pq.empty()) {
    const auto instruction = pq.top();
    pq.pop();

    for (int c = instruction.start; c < instruction.start + instruction.cycles; ++c) {
      int pixel_y = c / 40;
      int pixel_x = c % 40;
      if (abs(register_x - pixel_x) <= 1) {
        crt[pixel_y][pixel_x] = '#';
      }
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

  for (int y = 0; y < 6; ++y) {
    for (int x = 0; x < 40; ++x) {
      cout << crt[y][x];
    }
    cout << endl;
  }
}

int main() {
  Solve();
}