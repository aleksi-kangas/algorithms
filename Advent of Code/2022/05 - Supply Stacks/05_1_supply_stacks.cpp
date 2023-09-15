#include <cstdint>
#include <deque>
#include <iostream>
#include <string>
#include <vector>

struct Move {
  std::size_t from{0};
  std::size_t to{0};
  std::size_t count{0};
};

std::vector<std::deque<char>> ReadStacks() {
  constexpr std::size_t kRawStackLength{3};
  std::vector<std::deque<char>> stacks{};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    const std::size_t stack_count = line.size() / kRawStackLength;
    if (stacks.size() != stack_count) {
      stacks.resize(stack_count);
    }
    for (std::size_t i = 0; i < stack_count; ++i) {
      const std::size_t spacing = i * 1;
      const char c = line[i * kRawStackLength + spacing + 1];  // e.g. [Z] -> Z
      if (std::isalpha(c)) {
        stacks[i].push_back(c);
      }
    }
  }
  return stacks;
}

std::vector<Move> ReadMoves() {
  std::vector<Move> moves{};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    const auto count_label_index = line.find("move ");
    const auto from_label_index = line.find("from ", count_label_index + 5);
    const auto to_label_index = line.find("to ", from_label_index + 5);
    const std::size_t count = std::stoi(line.substr(count_label_index + 5, from_label_index));
    const std::size_t from = std::stoi(line.substr(from_label_index + 5, to_label_index));
    const std::size_t to = std::stoi(line.substr(to_label_index + 3));
    moves.push_back({from, to, count});
  }
  return moves;
}

void PerformMoves(std::vector<std::deque<char>>& stacks, const std::vector<Move>& moves) {
  for (const auto& move : moves) {
    std::deque<char>& from_stack = stacks[move.from - 1];
    std::deque<char>& to_stack = stacks[move.to - 1];
    for (std::size_t i = 0; i < move.count; ++i) {
      to_stack.push_front(from_stack.front());
      from_stack.pop_front();
    }
  }
}

int main() {
  auto stacks = ReadStacks();
  const auto moves = ReadMoves();
  PerformMoves(stacks, moves);
  for (const auto& stack : stacks) {
    if (!stack.empty()) {
      std::cout << stack.front();
    }
  }
  std::cout << std::endl;
}
