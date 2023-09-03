#include <cstdint>
#include <iostream>
#include <string>
#include <utility>

std::pair<std::int32_t, std::int32_t> ReadInitialPositions() {
  std::string p1{};
  std::getline(std::cin, p1);
  const auto p1_colon_index = p1.find(':');
  std::string p2{};
  std::getline(std::cin, p2);
  const auto p2_colon_index = p2.find(':');
  return {std::stoi(p1.substr(p1_colon_index + 2)), std::stoi(p2.substr(p2_colon_index + 2))};
}

std::pair<std::int32_t, std::int32_t> Play(std::pair<std::int32_t, std::int32_t> initial_positions,
                                           std::int32_t target_score, std::int32_t& roll_count) {
  roll_count = 0;
  std::pair<std::int32_t, std::int32_t> positions = std::move(initial_positions);
  std::pair<std::int32_t, std::int32_t> scores{};
  std::int32_t dice{1};
  bool p1{true};
  while (scores.first < target_score && scores.second < target_score) {
    std::int32_t moves{0};
    for (std::int32_t i = 1; i <= 3; ++i) {
      moves += dice++;
      if (dice == 101) {
        dice = 1;
      }
      ++roll_count;
    }
    if (p1) {
      positions.first += moves;
      positions.first %= 10;
      if (positions.first == 0) {
        positions.first = 10;
      }
      scores.first += positions.first;
    } else {
      positions.second += moves;
      positions.second %= 10;
      if (positions.second == 0) {
        positions.second = 10;
      }
      scores.second += positions.second;
    }
    p1 = !p1;
  }
  return scores;
}

int main() {
  const auto initial_positions = ReadInitialPositions();
  std::int32_t roll_count{0};
  const auto [p1_score, p2_score] = Play(initial_positions, 1000, roll_count);
  const auto answer = std::min(p1_score, p2_score) * roll_count;
  std::cout << answer << '\n';
}
