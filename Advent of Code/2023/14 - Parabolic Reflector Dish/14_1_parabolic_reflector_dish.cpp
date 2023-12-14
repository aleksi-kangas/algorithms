#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

class ParabolicReflectorDish {
 public:
  enum class Tile { kEmpty, kCubeRock, kRoundedRock };

  explicit ParabolicReflectorDish(std::vector<std::vector<Tile>> dish) : dish(std::move(dish)) {}

  [[nodiscard]] std::size_t TotalNorthLoad() const;

  [[nodiscard]] static ParabolicReflectorDish Parse(std::istream& is);

 private:
  std::vector<std::vector<Tile>> dish{};

  [[nodiscard]] std::size_t StepsNorth(const std::vector<std::vector<std::size_t>>& dp, std::size_t row,
                                       std::size_t column) const;
};

std::size_t ParabolicReflectorDish::TotalNorthLoad() const {
  std::vector dp(dish.size(), std::vector<std::size_t>(dish[0].size(), 0));
  for (std::size_t row = 1; row < dish.size(); ++row) {
    for (std::size_t column = 0; column < dish[row].size(); ++column) {
      switch (dish[row][column]) {
        case Tile::kEmpty:
        case Tile::kCubeRock:
          break;
        case Tile::kRoundedRock: {
          dp[row][column] = StepsNorth(dp, row, column);
        } break;
        default:
          throw std::logic_error{"Tile type not handled"};
      }
    }
  }
  std::size_t total_north_load{};
  for (std::size_t row = 0; row < dish.size(); ++row) {
    for (std::size_t column = 0; column < dish[row].size(); ++column) {
      switch (dish[row][column]) {
        case Tile::kEmpty:
        case Tile::kCubeRock:
          break;
        case Tile::kRoundedRock: {
          total_north_load += dish.size() - (row - dp[row][column]);
        } break;
        default:
          throw std::logic_error{"Tile type not handled"};
      }
    }
  }
  return total_north_load;
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

std::size_t ParabolicReflectorDish::StepsNorth(const std::vector<std::vector<std::size_t>>& dp, std::size_t row,
                                               std::size_t column) const {
  if (row == 0)
    return 0;
  for (std::int64_t i = static_cast<std::int32_t>(row) - 1; i >= 0; --i) {
    const auto steps_to_next = row - i - 1;
    switch (dish[i][column]) {
      case Tile::kEmpty:
        break;
      case Tile::kCubeRock:
        return steps_to_next;
      case Tile::kRoundedRock:
        return dp[i][column] + steps_to_next;
      default:
        throw std::logic_error{"Tile type not handled"};
    }
  }
  return row;
}

int main() {
  const auto dish = ParabolicReflectorDish::Parse(std::cin);
  std::cout << dish.TotalNorthLoad() << std::endl;
}
