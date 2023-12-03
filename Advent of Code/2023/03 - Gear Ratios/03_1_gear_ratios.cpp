#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> ReadEngineSchematic(std::istream& is) {
  std::vector<std::string> engine_schematic{};
  std::string line{};
  while (std::getline(is, line) && !line.empty()) {
    engine_schematic.push_back(std::move(line));
  }
  return engine_schematic;
}

bool IsSymbol(char c) {
  return !std::isdigit(c) && c != '.';
}

bool HasAdjacentSymbol(const std::vector<std::string>& engine_schematic, std::int32_t row, std::int32_t column) {
  for (const std::int32_t i : {-1, 0, 1}) {
    for (const std::int32_t j : {-1, 0, 1}) {
      if (row + i < 0 || engine_schematic.size() <= row + i || column + j < 0 || engine_schematic[0].size() <= column + j)
        continue;
      if (IsSymbol(engine_schematic[row + i][column + j])) {
        return true;
      }
    }
  }
  return false;
}

int main() {
  const std::vector<std::string> engine_schematic{ReadEngineSchematic(std::cin)};
  const std::int32_t n{static_cast<std::int32_t>(engine_schematic.size())};
  const std::int32_t m{static_cast<std::int32_t>(engine_schematic[0].size())};
  std::int32_t sum{0};
  std::int32_t number{0};
  bool is_part_number{false};
  for (std::int32_t row{0}; row < n; ++row) {
    for (std::int32_t column{0}; column < m; ++column) {
      if (std::isdigit(engine_schematic[row][column])) {
        const std::int32_t digit{engine_schematic[row][column] - '0'};
        number = number * 10 + digit;
        if (HasAdjacentSymbol(engine_schematic, row, column)) {
          is_part_number = true;
        }
      } else if (number > 0) {
        if (is_part_number) {
          sum += number;
        }
        number = 0;
        is_part_number = false;
      }
    }
  }
  std::cout << sum << std::endl;
}
