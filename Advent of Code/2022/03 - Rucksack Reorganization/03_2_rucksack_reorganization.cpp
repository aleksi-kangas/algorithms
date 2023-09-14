#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

std::vector<std::string> ReadGroup() {
  std::vector<std::string> group{};
  std::string line{};
  for (std::size_t i = 1; i <= 3; ++i) {
    std::getline(std::cin, line);
    group.push_back(std::move(line));
  }
  return group;
}

std::int32_t BadgePriority(const std::vector<std::string>& group) {
  std::unordered_map<char, std::int32_t> freq{};
  for (const std::string& elf : group) {
    std::unordered_set<char> seen{};
    for (const char c : elf) {
      seen.insert(c);
    }
    for (const char c : seen) {
      ++freq[c];
    }
  }
  for (const auto& [c, f] : freq) {
    if (f != 3)
      continue;
    if (c - 'a' >= 0 && c - 'a' <= 26)
      return c - 'a' + 1;
    if (c - 'A' >= 0 && c - 'A' <= 26)
      return c - 'A' + 27;
  }
  throw std::invalid_argument{"Expected one common item to exist"};
}

int main() {
  std::int32_t badge_sum{0};
  while (std::cin.peek() != '\n') {
    const std::vector<std::string> group = ReadGroup();
    badge_sum += BadgePriority(group);
  }
  std::cout << badge_sum << std::endl;
}
