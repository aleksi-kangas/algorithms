#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>

std::int32_t ReadElfCalories() {
  std::int32_t calories{0};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    const std::int32_t c = std::stoi(line);
    calories += c;
  }
  return calories;
}

int main() {
  std::int32_t answer{0};
  while (std::cin.peek() != '\n') {
    const std::int32_t elf_calories = ReadElfCalories();
    answer = std::max(answer, elf_calories);
  }
  std::cout << answer << std::endl;
}
