#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::int32_t> SeekLowPoints(const std::vector<std::vector<std::int32_t>>& m) {
  std::vector<std::int32_t> low_points{};
  for (std::size_t i = 0; i < m.size(); ++i) {
    for (std::size_t j = 0; j < m[i].size(); ++j) {
      const bool lower_than_above = i == 0 || m[i][j] < m[i - 1][j];
      const bool lower_than_below = i == m.size() - 1 || m[i][j] < m[i + 1][j];
      const bool lower_than_left = j == 0 || m[i][j] < m[i][j - 1];
      const bool lower_than_right = j == m[i].size() - 1 || m[i][j] < m[i][j + 1];
      if (lower_than_above && lower_than_below && lower_than_left && lower_than_right) {
        low_points.push_back(m[i][j]);
      }
    }
  }
  return low_points;
}

int main() {
  std::vector<std::vector<std::int32_t>> m{};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    m.emplace_back();
    m.back().reserve(line.size());
    for (const char c : line) {
      m.back().push_back(c - '0');
    }
  }
  const auto low_points = SeekLowPoints(m);
  std::int32_t sum{0};
  for (const auto low_point : low_points) {
    sum += low_point + 1;
  }
  std::cout << sum << std::endl;
}
