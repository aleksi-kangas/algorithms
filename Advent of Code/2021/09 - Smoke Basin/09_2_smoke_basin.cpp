#include <cstdint>
#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>

std::vector<std::pair<std::int32_t, std::int32_t>> SeekLowPoints(const std::vector<std::vector<std::int32_t>>& m) {
  std::vector<std::pair<std::int32_t, std::int32_t>> low_points{};
  for (std::size_t i = 0; i < m.size(); ++i) {
    for (std::size_t j = 0; j < m[i].size(); ++j) {
      const bool lower_than_above = i == 0 || m[i][j] < m[i - 1][j];
      const bool lower_than_below = i == m.size() - 1 || m[i][j] < m[i + 1][j];
      const bool lower_than_left = j == 0 || m[i][j] < m[i][j - 1];
      const bool lower_than_right = j == m[i].size() - 1 || m[i][j] < m[i][j + 1];
      if (lower_than_above && lower_than_below && lower_than_left && lower_than_right) {
        low_points.emplace_back(i, j);
      }
    }
  }
  return low_points;
}

std::int32_t BasinSize(const std::vector<std::vector<std::int32_t>>& m,
                       const std::pair<std::int32_t, std::int32_t>& low_point) {
  std::int32_t size{1};
  // BFS
  std::vector<std::vector<bool>> visited(m.size(), std::vector<bool>(m[0].size(), false));
  std::queue<std::pair<std::int32_t, std::int32_t>> q{};
  q.push(low_point);
  visited[low_point.first][low_point.second] = true;

  const auto IsVisited = [&visited](std::int32_t i, std::int32_t j) -> bool {
    return visited[i][j];
  };
  const auto Is9 = [&m](std::int32_t i, std::int32_t j) -> bool {
    return m[i][j] == 9;
  };
  const auto IsGreater = [&m](const std::pair<std::int32_t, std::int32_t>& target,
                              const std::pair<std::int32_t, std::int32_t>& reference) -> bool {
    return m[target.first][target.second] > m[reference.first][reference.second];
  };

  while (!q.empty()) {
    const auto [i, j] = q.front();
    q.pop();
    if (i > 0 && !IsVisited(i - 1, j) && !Is9(i - 1, j) && IsGreater({i - 1, j}, {i, j})) {
      visited[i - 1][j] = true;
      q.emplace(i - 1, j);
      ++size;
    }
    if (i < m.size() - 1 && !IsVisited(i + 1, j) && !Is9(i + 1, j) && IsGreater({i + 1, j}, {i, j})) {
      visited[i + 1][j] = true;
      q.emplace(i + 1, j);
      ++size;
    }
    if (j > 0 && !IsVisited(i, j - 1) && !Is9(i, j - 1) && IsGreater({i, j - 1}, {i, j})) {
      visited[i][j - 1] = true;
      q.emplace(i, j - 1);
      ++size;
    }
    if (j < m[i].size() - 1 && !IsVisited(i, j + 1) && !Is9(i, j + 1) && IsGreater({i, j + 1}, {i, j})) {
      visited[i][j + 1] = true;
      q.emplace(i, j + 1);
      ++size;
    }
  }
  return size;
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
  std::priority_queue<std::int32_t> basin_sizes{};
  for (const auto low_point : low_points) {
    basin_sizes.push(BasinSize(m, low_point));
  }
  std::int64_t answer{1};
  for (std::int32_t i = 1; i <= 3; ++i) {
    answer *= basin_sizes.top();
    basin_sizes.pop();
  }
  std::cout << answer << std::endl;
}
