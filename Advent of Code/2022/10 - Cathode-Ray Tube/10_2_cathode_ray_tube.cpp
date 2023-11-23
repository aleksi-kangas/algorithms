#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

struct Instruction {
  enum class Type { kNoOp, kAddX } type{Type::kNoOp};
  std::int32_t value{0};

  [[nodiscard]] static std::vector<Instruction> Parse(std::string_view s) {
    if (s.starts_with("noop")) {
      return {Instruction{Type::kNoOp, 0}};
    }
    if (s.starts_with("addx")) {
      return {Instruction{Type::kNoOp, 0}, Instruction{Type::kAddX, std::stoi(std::string{s.substr(4)})}};
    }
    throw std::runtime_error{"Invalid instruction"};
  }
};

struct Program {
  std::vector<Instruction> instructions{};

  [[nodiscard]] static Program Parse(std::istream& is) {
    std::string line{};
    Program program{};
    while (std::getline(is, line) && !line.empty()) {
      std::vector<Instruction> instructions = Instruction::Parse(line);
      program.instructions.insert(std::end(program.instructions), std::begin(instructions), std::end(instructions));
    }
    return program;
  }
};

int main() {
  const Program program = Program::Parse(std::cin);
  const std::unordered_set<std::int32_t> kInterestingCycles{20, 60, 100, 140, 180, 220};
  std::array<std::int32_t, 1> registers{{1}};
  std::int32_t cycle{1};
  std::int64_t total_signal_strength{0};
  std::string crt_line{};
  crt_line.reserve(40);
  std::int32_t crt_column{0};
  for (const auto& [type, value] : program.instructions) {
    if (kInterestingCycles.contains(cycle)) {
      const std::int64_t signal_strength = cycle * static_cast<std::int64_t>(registers[0]);
      total_signal_strength += signal_strength;
    }
    const bool should_draw = std::abs(registers[0] - crt_column) <= 1;
    crt_line += should_draw ? '#' : '.';
    switch (type) {
      case Instruction::Type::kNoOp:
        break;
      case Instruction::Type::kAddX:
        registers[0] += value;
        break;
      default:
        throw std::runtime_error{"Invalid instruction type"};
    }
    ++cycle;
    ++crt_column;
    if (crt_column % 40 == 0) {
      crt_column = 0;
      std::cout << crt_line << '\n';
      crt_line.clear();
    }
  }
}
