#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <ranges>
#include <sstream>
#include <string>

// On some platforms, need to redirect input from a file:
// <executable_name> < <input_file_name>, e.g. 07_1_the_treachery_of_whales < in.txt

std::vector<std::int32_t> ReadPositions() {
  std::vector<std::int32_t> positions{};
  std::string line{};
  std::cin >> line;
  std::stringstream ss{line};
  for (std::int32_t position; ss >> position;) {
    positions.emplace_back(position);
    if (ss.peek() == ',') {
      ss.ignore();
    }
  }
  return positions;
}

int main() {
  const auto initial_positions = ReadPositions();
  const auto [min_it, max_it] = std::ranges::minmax_element(initial_positions);
  const std::int32_t min = *min_it;
  const std::int32_t max = *max_it;
  auto answer = std::numeric_limits<std::int32_t>::max();
  for (std::int32_t candidate = min; candidate <= max; ++candidate) {
    std::int32_t sum{0};
    std::ranges::for_each(initial_positions, [&](std::int32_t p) {
      const std::int32_t d = std::abs(p - candidate);
      sum += d * (d + 1) / 2;
    });
    answer = std::min(answer, sum);
  }
  std::cout << answer << std::endl;
}
