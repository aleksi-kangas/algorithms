#include <algorithm>
#include <cstdint>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

std::vector<std::vector<std::int32_t>> ReadEnergies() {
  std::vector<std::vector<std::int32_t>> energies{};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    energies.emplace_back();
    energies.back().reserve(line.size());
    for (const char c : line) {
      energies.back().emplace_back(c - '0');
    }
  }
  return energies;
}

void IncrementAllEnergies(std::vector<std::vector<std::int32_t>>& energies) {
  std::ranges::for_each(energies, [](auto& row) { std::ranges::for_each(row, [](auto& energy) { ++energy; }); });
}

void IncrementNeighbors(std::vector<std::vector<std::int32_t>>& energies, std::int32_t i, std::int32_t j) {
  for (std::int32_t k = i - 1; k <= i + 1; ++k) {
    for (std::int32_t l = j - 1; l <= j + 1; ++l) {
      if (k < 0 || static_cast<std::int32_t>(energies.size()) <= k || l < 0 ||
          static_cast<std::int32_t>(energies[k].size()) <= l || (k == i && l == j))
        continue;
      ++energies[k][l];
    }
  }
}

std::int32_t Flash(std::vector<std::vector<std::int32_t>>& energies, std::vector<std::vector<bool>>& flashed,
                   std::int32_t i, std::int32_t j) {
  if (flashed[i][j] || energies[i][j] <= 9)
    return 0;
  flashed[i][j] = true;
  std::int32_t neighbors_flashed{0};
  IncrementNeighbors(energies, static_cast<std::int32_t>(i), static_cast<std::int32_t>(j));
  for (std::int32_t k = i - 1; k <= i + 1; ++k) {
    for (std::int32_t l = j - 1; l <= j + 1; ++l) {
      if (k < 0 || static_cast<std::int32_t>(energies.size()) <= k || l < 0 ||
          static_cast<std::int32_t>(energies[k].size()) <= l || (k == i && l == j))
        continue;
      neighbors_flashed += Flash(energies, flashed, k, l);
    }
  }
  return 1 + neighbors_flashed;
}

std::int32_t FlashAll(std::vector<std::vector<std::int32_t>>& energies) {
  std::int32_t total_flashes{0};
  std::vector<std::vector<bool>> flashed(energies.size(), std::vector<bool>(energies[0].size(), false));
  for (std::size_t i = 0; i < energies.size(); ++i) {
    for (std::size_t j = 0; j < energies[i].size(); ++j) {
      total_flashes += Flash(energies, flashed, static_cast<std::int32_t>(i), static_cast<std::int32_t>(j));
    }
  }
  for (std::size_t i = 0; i < energies.size(); ++i) {
    for (std::size_t j = 0; j < energies[i].size(); ++j) {
      if (flashed[i][j]) {
        energies[i][j] = 0;
      }
    }
  }
  return total_flashes;
}

int main() {
  auto energies = ReadEnergies();
  std::int32_t total_flashes{0};
  for (std::int32_t step = 1; step <= 100; ++step) {
    IncrementAllEnergies(energies);
    total_flashes += FlashAll(energies);
  }
  std::cout << total_flashes << '\n';
}
