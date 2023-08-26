#include <algorithm>
#include <charconv>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using Coordinates = std::pair<std::int32_t, std::int32_t>;

template <>
struct std::hash<Coordinates> {
  std::size_t operator()(const Coordinates& c) const {
    return (std::hash<std::int32_t>()(c.first) ^ (std::hash<std::int32_t>()(c.second) << 1)) >> 1;
  }
};

Coordinates ParseCoordinates(std::string_view s) {
  const auto comma_index = s.find(',');
  if (comma_index == std::string_view::npos)
    throw std::invalid_argument{"Invalid coordinates"};
  const auto ParseCoordinate = [&](std::string_view coordinate_s) -> std::int32_t {
    std::int32_t coordinate{0};
    const auto [_, ec] = std::from_chars(coordinate_s.data(), coordinate_s.data() + coordinate_s.size(), coordinate);
    if (ec != std::errc{})
      throw std::invalid_argument{"Invalid coordinates"};
    return coordinate;
  };
  return {ParseCoordinate(s.substr(0, comma_index)), ParseCoordinate(s.substr(comma_index + 1))};
}

std::vector<std::pair<Coordinates, Coordinates>> ReadLines() {
  std::vector<std::pair<Coordinates, Coordinates>> lines{};
  for (std::string line; std::getline(std::cin, line);) {
    if (line.empty())
      break;
    constexpr auto kArrow{" -> "};
    constexpr auto kArrowSize{4};
    const auto arrow_index = line.find(kArrow);
    if (arrow_index == std::string::npos)
      throw std::invalid_argument{"Invalid line"};
    const auto from = ParseCoordinates(line.substr(0, arrow_index));
    const auto to = ParseCoordinates(line.substr(arrow_index + kArrowSize));
    lines.emplace_back(from, to);
  }
  return lines;
}

bool IsHorizontal(const Coordinates& from, const Coordinates& to) {
  return from.second == to.second;
}

bool IsVertical(const Coordinates& from, const Coordinates& to) {
  return from.first == to.first;
}

bool IsDiagonal(const Coordinates& from, const Coordinates& to) {
  const std::int32_t dx = std::abs(from.first - to.first);
  const std::int32_t dy = std::abs(from.second - to.second);
  return dx == dy;
}

int main() {
  const auto lines = ReadLines();
  std::unordered_map<Coordinates, std::int32_t> freq{};
  for (const auto& [from, to] : lines) {
    if (IsHorizontal(from, to) || IsVertical(from, to)) {
      for (std::int32_t x = std::min(from.first, to.first); x <= std::max(from.first, to.first); ++x) {
        for (std::int32_t y = std::min(from.second, to.second); y <= std::max(from.second, to.second); ++y) {
          ++freq[{x, y}];
        }
      }
    } else if (IsDiagonal(from, to)) {
      const std::int32_t dx = from.first < to.first ? 1 : -1;
      const std::int32_t dy = from.second < to.second ? 1 : -1;
      for (std::int32_t i = 0; i <= std::abs(from.first - to.first); ++i) {
        const std::int32_t x = from.first + i * dx;
        const std::int32_t y = from.second + i * dy;
        ++freq[{x, y}];
      }
    }
  }
  std::int32_t answer{0};
  std::ranges::for_each(freq, [&answer](const auto& p) {
    const auto& [_, count] = p;
    if (count > 1) {
      ++answer;
    }
  });
  std::cout << answer << std::endl;
}
