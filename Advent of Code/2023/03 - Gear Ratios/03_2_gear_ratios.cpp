#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using position_t = std::pair<std::int32_t, std::int32_t>;

template <>
struct std::hash<position_t> {
  std::size_t operator()(const position_t& position) const noexcept {
    const std::size_t h1 = std::hash<std::int32_t>{}(position.first);
    const std::size_t h2 = std::hash<std::int32_t>{}(position.second);
    return h1 ^ (h2 << 1);
  }
};

std::vector<std::string> ReadEngineSchematic(std::istream& is) {
  std::vector<std::string> engine_schematic{};
  std::string line{};
  while (std::getline(is, line) && !line.empty()) {
    engine_schematic.push_back(std::move(line));
  }
  return engine_schematic;
}

bool IsGear(char c) {
  return c == '*';
}

std::unordered_set<position_t> AdjacentGears(const std::vector<std::string>& engine_schematic, std::int32_t row,
                                             std::int32_t column) {
  std::unordered_set<position_t> adjacent_gears{};
  for (const std::int32_t i : {-1, 0, 1}) {
    for (const std::int32_t j : {-1, 0, 1}) {
      if (row + i < 0 || engine_schematic.size() <= row + i || column + j < 0 ||
          engine_schematic[0].size() <= column + j)
        continue;
      if (IsGear(engine_schematic[row + i][column + j])) {
        adjacent_gears.emplace(row + i, column + j);
      }
    }
  }
  return adjacent_gears;
}

std::int64_t GearRatio(const std::vector<std::int32_t>& adjacent_numbers) {
  return adjacent_numbers.size() == 2 ? static_cast<std::int64_t>(adjacent_numbers[0]) * adjacent_numbers[1] : 0;
}

int main() {
  const std::vector<std::string> engine_schematic = ReadEngineSchematic(std::cin);
  const std::int32_t n{static_cast<std::int32_t>(engine_schematic.size())};
  const std::int32_t m{static_cast<std::int32_t>(engine_schematic[0].size())};
  std::int32_t sum{0};
  std::int32_t number{0};
  std::unordered_map<position_t, std::vector<std::int32_t>> gear_adjacent_numbers{};
  std::unordered_set<position_t> gears{};
  for (std::int32_t row{0}; row < n; ++row) {
    for (std::int32_t column{0}; column < m; ++column) {
      if (std::isdigit(engine_schematic[row][column])) {
        const std::int32_t digit{engine_schematic[row][column] - '0'};
        number = number * 10 + digit;
        const auto adjacent_gears = AdjacentGears(engine_schematic, row, column);
        gears.insert(adjacent_gears.begin(), adjacent_gears.end());
      } else if (number > 0) {
        for (const auto& gear : gears) {
          gear_adjacent_numbers[gear].push_back(number);
        }
        number = 0;
        gears.clear();
      }
    }
  }
  std::int64_t gear_ratio_sum{};
  for (const auto& [gear, adjacent_numbers] : gear_adjacent_numbers) {
    gear_ratio_sum += GearRatio(adjacent_numbers);
  }
  std::cout << gear_ratio_sum << std::endl;
}
