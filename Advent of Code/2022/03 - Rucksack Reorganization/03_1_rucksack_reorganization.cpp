#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_set>

std::int32_t CommonItemPriority(std::string_view rucksack) {
  if (rucksack.size() % 2 != 0)
    throw std::invalid_argument{"Compartments must be of the same size"};
  const std::size_t compartment_size = rucksack.size() / 2;
  std::unordered_set<char> first_compartment{};
  for (std::size_t i = 0; i < compartment_size; ++i) {
    first_compartment.insert(rucksack[i]);
  }
  for (std::size_t i = 0; i < compartment_size; ++i) {
    if (!first_compartment.contains(rucksack[compartment_size + i]))
      continue;
    const char c = rucksack[compartment_size + i];
    if (c - 'a' >= 0 && c - 'a' <= 26)
      return c - 'a' + 1;
    if (c - 'A' >= 0 && c - 'A' <= 26)
      return c - 'A' + 27;
    throw std::invalid_argument{"Expected a alphabetic character"};
  }
  throw std::invalid_argument{"Expected one common item to exist"};
}

int main() {
  std::int32_t priority_sum{0};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    priority_sum += CommonItemPriority(line);
  }
  std::cout << priority_sum << std::endl;
}
