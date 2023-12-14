#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class ParabolicReflectorDish {
 public:
  enum class Tile { kEmpty, kCubeRock, kRoundedRock };

  explicit ParabolicReflectorDish(std::vector<std::vector<Tile>> dish) : dish(std::move(dish)) {}

  [[nodiscard]] std::string ToString() const;

  [[nodiscard]] std::size_t TotalNorthLoad() const;

  void SpinCycles(std::size_t cycles, std::unordered_map<std::string, std::size_t>& cache);

  [[nodiscard]] static ParabolicReflectorDish Parse(std::istream& is);

 private:
  std::vector<std::vector<Tile>> dish{};

  void TiltNorth();
  void TiltWest();
  void TiltSouth();
  void TiltEast();

  [[nodiscard]] std::size_t StepsNorth(const std::vector<std::vector<std::size_t>>& dp, std::size_t row,
                                       std::size_t column) const;
  [[nodiscard]] std::size_t StepsWest(const std::vector<std::vector<std::size_t>>& dp, std::size_t row,
                                      std::size_t column) const;
  [[nodiscard]] std::size_t StepsSouth(const std::vector<std::vector<std::size_t>>& dp, std::size_t row,
                                       std::size_t column) const;
  [[nodiscard]] std::size_t StepsEast(const std::vector<std::vector<std::size_t>>& dp, std::size_t row,
                                      std::size_t column) const;
};

std::string ParabolicReflectorDish::ToString() const {
  std::string s{};
  for (const auto& row : dish) {
    for (const auto& tile : row) {
      switch (tile) {
        case Tile::kEmpty:
          s += '.';
          break;
        case Tile::kCubeRock:
          s += '#';
          break;
        case Tile::kRoundedRock:
          s += 'O';
          break;
        default:
          throw std::logic_error{"Tile type not handled"};
      }
    }
    s += '\n';
  }
  return s;
}

std::size_t ParabolicReflectorDish::TotalNorthLoad() const {
  std::size_t total_north_load{0};
  for (std::size_t row{0}; row < dish.size(); ++row) {
    for (std::size_t column{0}; column < dish[row].size(); ++column) {
      switch (dish[row][column]) {
        case Tile::kEmpty:
        case Tile::kCubeRock:
          break;
        case Tile::kRoundedRock: {
          total_north_load += dish.size() - row;
        } break;
        default:
          throw std::logic_error{"Tile type not handled"};
      }
    }
  }
  return total_north_load;
}

void ParabolicReflectorDish::SpinCycles(std::size_t cycles, std::unordered_map<std::string, std::size_t>& cache) {
  std::optional<std::size_t> remaining_cycles{std::nullopt};
  for (std::size_t cycle{0}; cycle < cycles; ++cycle) {
    const auto cache_it = cache.find(ToString());
    if (cache_it != cache.end()) {
      const std::size_t cycle_length = cycle - cache_it->second;
      remaining_cycles = (cycles - cycle) % cycle_length;
      break;
    }
    cache[ToString()] = cycle;
    TiltNorth();
    TiltWest();
    TiltSouth();
    TiltEast();
  }
  if (remaining_cycles.has_value()) {
    for (std::size_t cycle{0}; cycle < remaining_cycles.value(); ++cycle) {
      TiltNorth();
      TiltWest();
      TiltSouth();
      TiltEast();
    }
  }
}

ParabolicReflectorDish ParabolicReflectorDish::Parse(std::istream& is) {
  std::vector<std::vector<Tile>> dish{};
  std::string line{};
  while (std::getline(is, line) && !line.empty()) {
    std::vector<Tile> row{};
    for (const char c : line) {
      switch (c) {
        case '.':
          row.push_back(Tile::kEmpty);
          break;
        case '#':
          row.push_back(Tile::kCubeRock);
          break;
        case 'O':
          row.push_back(Tile::kRoundedRock);
          break;
        default:
          throw std::invalid_argument{"Invalid tile type"};
      }
    }
    dish.push_back(std::move(row));
  }
  return ParabolicReflectorDish{std::move(dish)};
}

void ParabolicReflectorDish::TiltNorth() {
  std::vector north_dp{dish.size(), std::vector<std::size_t>(dish[0].size(), 0)};
  for (std::size_t row{1}; row < dish.size(); ++row) {
    for (std::size_t column{0}; column < dish[row].size(); ++column) {
      switch (dish[row][column]) {
        case Tile::kEmpty:
        case Tile::kCubeRock:
          break;
        case Tile::kRoundedRock: {
          north_dp[row][column] = StepsNorth(north_dp, row, column);
        } break;
        default:
          throw std::logic_error{"Tile type not handled"};
      }
    }
  }
  for (std::size_t row{0}; row < dish.size(); ++row) {
    for (std::size_t column{0}; column < dish[row].size(); ++column) {
      switch (dish[row][column]) {
        case Tile::kEmpty:
        case Tile::kCubeRock:
          break;
        case Tile::kRoundedRock: {
          dish[row][column] = Tile::kEmpty;
          dish[row - north_dp[row][column]][column] = Tile::kRoundedRock;
        } break;
        default:
          throw std::logic_error{"Tile type not handled"};
      }
    }
  }
}

void ParabolicReflectorDish::TiltWest() {
  std::vector west_dp{dish.size(), std::vector<std::size_t>(dish[0].size(), 0)};
  for (std::size_t column{1}; column < dish[0].size(); ++column) {
    for (std::size_t row{0}; row < dish.size(); ++row) {
      switch (dish[row][column]) {
        case Tile::kEmpty:
        case Tile::kCubeRock:
          break;
        case Tile::kRoundedRock: {
          west_dp[row][column] = StepsWest(west_dp, row, column);
        } break;
        default:
          throw std::logic_error{"Tile type not handled"};
      }
    }
  }
  for (std::size_t column{0}; column < dish[0].size(); ++column) {
    for (std::size_t row{0}; row < dish.size(); ++row) {
      switch (dish[row][column]) {
        case Tile::kEmpty:
        case Tile::kCubeRock:
          break;
        case Tile::kRoundedRock: {
          dish[row][column] = Tile::kEmpty;
          dish[row][column - west_dp[row][column]] = Tile::kRoundedRock;
        } break;
        default:
          throw std::logic_error{"Tile type not handled"};
      }
    }
  }
}

void ParabolicReflectorDish::TiltSouth() {
  std::vector south_dp{dish.size(), std::vector<std::size_t>(dish[0].size(), 0)};
  for (std::int64_t row{static_cast<std::int32_t>(dish.size()) - 2}; row >= 0; --row) {
    for (std::size_t column{0}; column < dish[row].size(); ++column) {
      switch (dish[row][column]) {
        case Tile::kEmpty:
        case Tile::kCubeRock:
          break;
        case Tile::kRoundedRock: {
          south_dp[row][column] = StepsSouth(south_dp, row, column);
        } break;
        default:
          throw std::logic_error{"Tile type not handled"};
      }
    }
  }
  for (std::int64_t row{static_cast<std::int32_t>(dish.size()) - 1}; row >= 0; --row) {
    for (std::size_t column{0}; column < dish[row].size(); ++column) {
      switch (dish[row][column]) {
        case Tile::kEmpty:
        case Tile::kCubeRock:
          break;
        case Tile::kRoundedRock: {
          dish[row][column] = Tile::kEmpty;
          dish[row + south_dp[row][column]][column] = Tile::kRoundedRock;
        } break;
        default:
          throw std::logic_error{"Tile type not handled"};
      }
    }
  }
}

void ParabolicReflectorDish::TiltEast() {
  std::vector east_dp{dish.size(), std::vector<std::size_t>(dish[0].size(), 0)};
  for (std::int64_t column{static_cast<std::int32_t>(dish[0].size()) - 2}; column >= 0; --column) {
    for (std::size_t row{0}; row < dish.size(); ++row) {
      switch (dish[row][column]) {
        case Tile::kEmpty:
        case Tile::kCubeRock:
          break;
        case Tile::kRoundedRock: {
          east_dp[row][column] = StepsEast(east_dp, row, column);
        } break;
        default:
          throw std::logic_error{"Tile type not handled"};
      }
    }
  }
  for (std::int64_t column{static_cast<std::int32_t>(dish[0].size()) - 1}; column >= 0; --column) {
    for (std::size_t row{0}; row < dish.size(); ++row) {
      switch (dish[row][column]) {
        case Tile::kEmpty:
        case Tile::kCubeRock:
          break;
        case Tile::kRoundedRock: {
          dish[row][column] = Tile::kEmpty;
          dish[row][column + east_dp[row][column]] = Tile::kRoundedRock;
        } break;
        default:
          throw std::logic_error{"Tile type not handled"};
      }
    }
  }
}

std::size_t ParabolicReflectorDish::StepsNorth(const std::vector<std::vector<std::size_t>>& dp, std::size_t row,
                                               std::size_t column) const {
  if (row == 0)
    return 0;
  for (std::int64_t r{static_cast<std::int32_t>(row) - 1}; r >= 0; --r) {
    const auto steps_to_next = row - r - 1;
    switch (dish[r][column]) {
      case Tile::kEmpty:
        break;
      case Tile::kCubeRock:
        return steps_to_next;
      case Tile::kRoundedRock:
        return dp[r][column] + steps_to_next;
      default:
        throw std::logic_error{"Tile type not handled"};
    }
  }
  return row;
}

std::size_t ParabolicReflectorDish::StepsWest(const std::vector<std::vector<std::size_t>>& dp, std::size_t row,
                                              std::size_t column) const {
  if (column == 0)
    return 0;
  for (std::int64_t c{static_cast<std::int32_t>(column) - 1}; c >= 0; --c) {
    const auto steps_to_next = column - c - 1;
    switch (dish[row][c]) {
      case Tile::kEmpty:
        break;
      case Tile::kCubeRock:
        return steps_to_next;
      case Tile::kRoundedRock:
        return dp[row][c] + steps_to_next;
      default:
        throw std::logic_error{"Tile type not handled"};
    }
  }
  return column;
}

std::size_t ParabolicReflectorDish::StepsSouth(const std::vector<std::vector<std::size_t>>& dp, std::size_t row,
                                               std::size_t column) const {
  if (row == dish.size() - 1)
    return 0;
  for (std::size_t r{row + 1}; r < dish.size(); ++r) {
    const auto steps_to_next = r - row - 1;
    switch (dish[r][column]) {
      case Tile::kEmpty:
        break;
      case Tile::kCubeRock:
        return steps_to_next;
      case Tile::kRoundedRock:
        return dp[r][column] + steps_to_next;
      default:
        throw std::logic_error{"Tile type not handled"};
    }
  }
  return dish.size() - row - 1;
}

std::size_t ParabolicReflectorDish::StepsEast(const std::vector<std::vector<std::size_t>>& dp, std::size_t row,
                                              std::size_t column) const {
  if (column == dish[row].size() - 1)
    return 0;
  for (std::size_t c{column + 1}; c < dish[row].size(); ++c) {
    const auto steps_to_next = c - column - 1;
    switch (dish[row][c]) {
      case Tile::kEmpty:
        break;
      case Tile::kCubeRock:
        return steps_to_next;
      case Tile::kRoundedRock:
        return dp[row][c] + steps_to_next;
      default:
        throw std::logic_error{"Tile type not handled"};
    }
  }
  return dish[row].size() - column - 1;
}

int main() {
  auto dish = ParabolicReflectorDish::Parse(std::cin);
  std::unordered_map<std::string, std::size_t> cache{};
  dish.SpinCycles(1000000000, cache);
  std::cout << dish.TotalNorthLoad() << std::endl;
}
