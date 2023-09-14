#include <algorithm>
#include <cstdint>
#include <iostream>
#include <queue>
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
  std::priority_queue<std::int32_t> elves{};
  while (std::cin.peek() != '\n') {
    const std::int32_t elf_calories = ReadElfCalories();
    elves.push(elf_calories);
  }
  std::int32_t answer{0};
  for (std::size_t i = 1; i <= 3; ++i) {
    answer += elves.top();
    elves.pop();
  }
  std::cout << answer << std::endl;
}
