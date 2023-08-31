#include <array>
#include <cstdint>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <utility>
#include <vector>

using position_t = std::pair<std::size_t, std::size_t>;
using pq_element_t = std::pair<std::int32_t, position_t>;

std::vector<std::vector<std::int32_t>> ReadRisks() {
  std::vector<std::vector<std::int32_t>> risks{};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    risks.emplace_back();
    risks.back().reserve(line.size());
    for (const char c : line) {
      risks.back().push_back(c - '0');
    }
  }
  return risks;
}

std::vector<std::vector<std::int32_t>> MinimumTotalRisk(const std::vector<std::vector<std::int32_t>>& risks) {
  const std::size_t n = risks.size();
  const std::size_t m = risks[0].size();
  std::vector<std::vector<std::int32_t>> minimum_total_risks(
      n, std::vector<std::int32_t>(m, std::numeric_limits<std::int32_t>::max()));
  minimum_total_risks[0][0] = 0;
  std::priority_queue<pq_element_t, std::vector<pq_element_t>, std::greater<>> mpq{};
  for (std::size_t i = 0; i < n; ++i) {
    for (std::size_t j = 0; j < m; ++j) {
      mpq.emplace(minimum_total_risks[i][j], position_t{i, j});
    }
  }

  while (!mpq.empty()) {
    const auto [current_risk, current_position] = mpq.top();
    mpq.pop();
    // C++ Priority Queue implementation does not support the 'change key' operation out of the box.
    // Instead, we'll always push the position with the new key, and let the old key be removed later by the following if-clause.
    if (current_risk > minimum_total_risks[current_position.first][current_position.second])
      continue;
    constexpr std::array<std::int32_t, 4> kDx{-1, 0, 1, 0};
    constexpr std::array<std::int32_t, 4> kDy{0, -1, 0, 1};
    for (std::int32_t i = 0; i < 4; ++i) {
      const std::int32_t y = static_cast<std::int32_t>(current_position.first) + kDy[i];
      const std::int32_t x = static_cast<std::int32_t>(current_position.second) + kDx[i];
      if (y < 0 || n <= y || x < 0 || m <= x)
        continue;
      const auto new_total_minimum_risk = current_risk + risks[y][x];
      if (new_total_minimum_risk < minimum_total_risks[y][x]) {
        minimum_total_risks[y][x] = new_total_minimum_risk;
        mpq.emplace(new_total_minimum_risk, position_t{y, x});
      }
    }
  }
  return minimum_total_risks;
}

int main() {
  const auto risks = ReadRisks();
  const auto minimum_total_risks = MinimumTotalRisk(risks);
  std::cout << minimum_total_risks.back().back() << '\n';
}
