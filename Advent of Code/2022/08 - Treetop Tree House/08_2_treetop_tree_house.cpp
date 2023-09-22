#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::vector<std::int32_t>> ReadGrid() {
  std::vector<std::vector<std::int32_t>> grid{};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    std::vector<std::int32_t> row{};
    for (const char c : line) {
      row.push_back(c - '0');
    }
    grid.push_back(std::move(row));
  }
  return grid;
}

std::int32_t VisibleLeft(const std::vector<std::vector<std::int32_t>>& grid, std::size_t i, std::size_t j) {
  std::int32_t count{0};
  for (std::int32_t k = static_cast<std::int32_t>(j) - 1; k >= 0; --k) {
    ++count;
    if (grid[i][j] <= grid[i][k])
      return count;
  }
  return count;
}

std::int32_t VisibleRight(const std::vector<std::vector<std::int32_t>>& grid, std::size_t i, std::size_t j) {
  std::int32_t count{0};
  for (std::size_t k = j + 1; k < grid[0].size(); ++k) {
    ++count;
    if (grid[i][j] <= grid[i][k])
      return count;
  }
  return count;
}

std::int32_t VisibleUp(const std::vector<std::vector<std::int32_t>>& grid, std::size_t i, std::size_t j) {
  std::int32_t count{0};
  for (std::int32_t k = static_cast<std::int32_t>(i) - 1; k >= 0; --k) {
    ++count;
    if (grid[i][j] <= grid[k][j])
      return count;
  }
  return count;
}

std::int32_t VisibleDown(const std::vector<std::vector<std::int32_t>>& grid, std::size_t i, std::size_t j) {
  std::int32_t count{0};
  for (std::size_t k = i + 1; k < grid.size(); ++k) {
    ++count;
    if (grid[i][j] <= grid[k][j])
      return count;
  }
  return count;
}

int main() {
  auto grid = ReadGrid();
  const std::size_t n = grid.size();
  const std::size_t m = grid[0].size();
  std::int64_t answer{0};
  for (std::size_t i = 0; i < n; ++i) {
    for (std::size_t j = 0; j < m; ++j) {
      std::int64_t score{1};
      score *= VisibleLeft(grid, i, j);
      score *= VisibleRight(grid, i, j);
      score *= VisibleUp(grid, i, j);
      score *= VisibleDown(grid, i, j);
      answer = std::max(answer, score);
    }
  }
  std::cout << answer << std::endl;
}
