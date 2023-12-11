#include <cstddef>
#include <iostream>
#include <istream>
#include <ranges>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using position_t = std::pair<std::size_t, std::size_t>;

template <>
struct std::hash<position_t> {
  std::size_t operator()(const position_t& p) const noexcept {
    const std::size_t h1 = std::hash<std::size_t>{}(p.first);
    const std::size_t h2 = std::hash<std::size_t>{}(p.second);
    return h1 ^ (h2 << 1);
  }
};

struct SpaceMap {
  std::unordered_set<position_t> galaxies{};
  std::unordered_map<std::size_t, std::size_t> empty_row_count{};
  std::unordered_map<std::size_t, std::size_t> empty_column_count{};

  [[nodiscard]] position_t RealPosition(const position_t& p) const;

  [[nodiscard]] std::size_t PairWiseDistance(const position_t& lhs, const position_t& rhs) const;

  [[nodiscard]] std::size_t PairWiseDistanceSum() const;

  [[nodiscard]] static SpaceMap Parse(std::istream& is);
};

position_t SpaceMap::RealPosition(const position_t& p) const {
  const auto empty_row_count_it = empty_row_count.find(p.first);
  const auto empty_column_count_it = empty_column_count.find(p.second);
  return {p.first + (empty_row_count_it != empty_row_count.end() ? empty_row_count_it->second * (1000000 - 1) : 0),
          p.second +
              (empty_column_count_it != empty_column_count.end() ? empty_column_count_it->second * (1000000 - 1) : 0)};
}

std::size_t SpaceMap::PairWiseDistance(const position_t& lhs, const position_t& rhs) const {
  const auto real_lhs = RealPosition(lhs);
  const auto real_rhs = RealPosition(rhs);
  return std::abs(static_cast<std::int64_t>(real_lhs.first) - static_cast<std::int64_t>(real_rhs.first)) +
         std::abs(static_cast<std::int64_t>(real_lhs.second) - static_cast<std::int64_t>(real_rhs.second));
}

std::size_t SpaceMap::PairWiseDistanceSum() const {
  std::size_t distance_sum{0};
  for (const auto& lhs : galaxies) {
    for (const auto& rhs : galaxies) {
      distance_sum += PairWiseDistance(lhs, rhs);
    }
  }
  return distance_sum / 2;
}

SpaceMap SpaceMap::Parse(std::istream& is) {
  SpaceMap space_map{};
  std::string line{};
  std::size_t empty_row_count{0};
  std::size_t max_row{0};
  std::size_t max_column{0};
  for (std::size_t r{0}; std::getline(is, line) && !line.empty(); ++r) {
    bool is_empty_row{true};
    for (std::size_t c{0}; c < line.size(); ++c) {
      if (line[c] == '#') {
        space_map.galaxies.emplace(r, c);
        is_empty_row = false;
      }
      max_column = std::max(max_column, c);
    }
    if (is_empty_row) {
      ++empty_row_count;
    }
    space_map.empty_row_count[r] = empty_row_count;
    max_row = std::max(max_row, r);
  }
  std::size_t empty_column_count{0};
  for (std::size_t c{0}; c <= max_column; ++c) {
    bool is_empty_column{true};
    for (std::size_t r{0}; r <= max_row; ++r) {
      if (space_map.galaxies.contains({r, c})) {
        is_empty_column = false;
        break;
      }
    }
    if (is_empty_column) {
      ++empty_column_count;
    }
    space_map.empty_column_count[c] = empty_column_count;
  }
  return space_map;
}

int main() {
  const auto galaxies = SpaceMap::Parse(std::cin);
  std::cout << galaxies.PairWiseDistanceSum() << std::endl;
}
