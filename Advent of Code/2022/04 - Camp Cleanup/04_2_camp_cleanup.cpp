#include <cstdint>
#include <iostream>
#include <string>
#include <utility>

using range_t = std::pair<std::int32_t, std::int32_t>;

std::pair<range_t, range_t> ParseRangePair(const std::string& s) {
  const auto first_dash_index = s.find('-');
  const auto comma_index = s.find(',');
  const auto second_dash_index = s.find('-', first_dash_index + 1);
  const auto first_begin = s.substr(0, first_dash_index);
  const auto first_end = s.substr(first_dash_index + 1, comma_index - first_dash_index - 1);
  const auto second_begin = s.substr(comma_index + 1, second_dash_index - comma_index - 1);
  const auto second_end = s.substr(second_dash_index + 1);
  return {std::make_pair(std::stoi(first_begin), std::stoi(first_end)),
          std::make_pair(std::stoi(second_begin), std::stoi(second_end))};
}

bool IsOverlapping(const std::pair<range_t, range_t>& p) {
  const auto& [a, b] = p;
  return a.first <= b.first && b.first <= a.second || a.first <= b.second && b.second <= a.second ||
         b.first <= a.first && a.first <= b.second || b.first <= a.second && a.second <= b.second;
}

int main() {
  std::int32_t answer{0};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    const auto range_pair = ParseRangePair(line);
    if (IsOverlapping(range_pair)) {
      ++answer;
    }
  }
  std::cout << answer << std::endl;
}
