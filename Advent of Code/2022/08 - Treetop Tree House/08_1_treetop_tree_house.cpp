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

bool IsVisibleLeft(const std::vector<std::vector<std::int32_t>>& grid, std::size_t i, std::size_t j) {
  for (std::int32_t k = static_cast<std::int32_t>(j) - 1; k >= 0; --k) {
    if (grid[i][j] <= grid[i][k])
      return false;
  }
  return true;
}

bool IsVisibleRight(const std::vector<std::vector<std::int32_t>>& grid, std::size_t i, std::size_t j) {
  for (std::size_t k = j + 1; k < grid[0].size(); ++k) {
    if (grid[i][j] <= grid[i][k])
      return false;
  }
  return true;
}

bool IsVisibleUp(const std::vector<std::vector<std::int32_t>>& grid, std::size_t i, std::size_t j) {
  for (std::int32_t k = static_cast<std::int32_t>(i) - 1; k >= 0; --k) {
    if (grid[i][j] <= grid[k][j])
      return false;
  }
  return true;
}

bool IsVisibleDown(const std::vector<std::vector<std::int32_t>>& grid, std::size_t i, std::size_t j) {
  for (std::size_t k = i + 1; k < grid.size(); ++k) {
    if (grid[i][j] <= grid[k][j])
      return false;
  }
  return true;
}

int main() {
  auto grid = ReadGrid();
  const std::size_t n = grid.size();
  const std::size_t m = grid[0].size();
  std::vector<std::vector<bool>> visible(n, std::vector<bool>(m, false));
  for (std::size_t i = 0; i < n; ++i) {
    for (std::size_t j = 0; j < m; ++j) {
      visible[i][j] = IsVisibleLeft(grid, i, j) || IsVisibleRight(grid, i, j) || IsVisibleUp(grid, i, j) ||
                      IsVisibleDown(grid, i, j);
    }
  }
  std::int32_t answer{0};
  for (std::size_t i = 0; i < n; ++i) {
    for (std::size_t j = 0; j < m; ++j) {
      if (visible[i][j]) {
        ++answer;
      }
    }
  }
  std::cout << answer << std::endl;
}
