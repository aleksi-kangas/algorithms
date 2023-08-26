#include <algorithm>
#include <cstdint>
#include <iostream>
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
  auto initial_positions = ReadPositions();
  const auto n = static_cast<std::int32_t>(initial_positions.size());
  std::ranges::nth_element(initial_positions, initial_positions.begin() + n / 2);
  const std::int32_t median = initial_positions[n / 2];
  std::int32_t answer{0};
  std::ranges::for_each(initial_positions, [&](std::int32_t position) { answer += std::abs(position - median); });
  std::cout << answer << std::endl;
}
