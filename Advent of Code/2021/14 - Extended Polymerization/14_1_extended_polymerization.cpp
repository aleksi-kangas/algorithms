#include <cstdint>
#include <iostream>
#include <limits>
#include <list>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

template <>
struct std::hash<std::pair<char, char>> {
  std::size_t operator()(const std::pair<char, char>& p) const noexcept {
    const std::size_t h1 = std::hash<char>{}(p.first);
    const std::size_t h2 = std::hash<char>{}(p.second);
    return h1 ^ (h2 << 1);
  }
};

std::list<char> ReadPolymerTemplate() {
  std::list<char> polymer_template{};
  std::string line{};
  std::getline(std::cin, line);
  for (const auto c : line) {
    polymer_template.emplace_back(c);
  }
  return polymer_template;
}

std::unordered_map<std::pair<char, char>, char> ReadInsertionRules() {
  std::unordered_map<std::pair<char, char>, char> rules{};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    constexpr std::string_view kArrow{" -> "};
    const auto arrow_index = line.find(kArrow);
    if (arrow_index == std::string::npos)
      throw std::invalid_argument{"Invalid rule"};
    rules[{line[0], line[1]}] = line[arrow_index + kArrow.size()];
  }
  return rules;
}

void InsertionStep(std::list<char>& polymer_template, const std::unordered_map<std::pair<char, char>, char>& rules) {
  auto previous = polymer_template.begin();
  for (auto current = std::next(previous); current != polymer_template.end(); ++current) {
    if (const auto it = rules.find({*previous, *current}); it != rules.end()) {
      polymer_template.insert(current, it->second);
    }
    previous = current;
  }
}

std::pair<std::int32_t, std::int32_t> MinMaxFrequency(const std::list<char>& polymer_template) {
  std::unordered_map<char, std::int32_t> frequency{};
  for (const auto c : polymer_template) {
    ++frequency[c];
  }
  std::int32_t min_frequency{std::numeric_limits<std::int32_t>::max()};
  std::int32_t max_frequency{std::numeric_limits<std::int32_t>::min()};
  for (const auto [_, f] : frequency) {
    min_frequency = std::min(min_frequency, f);
    max_frequency = std::max(max_frequency, f);
  }
  return {min_frequency, max_frequency};
}

int main() {
  auto polymer_template = ReadPolymerTemplate();
  std::cin.ignore();
  const auto rules = ReadInsertionRules();
  for (std::size_t step = 1; step <= 10; ++step) {
    InsertionStep(polymer_template, rules);
  }
  const auto [min_frequency, max_frequency] = MinMaxFrequency(polymer_template);
  std::cout << max_frequency - min_frequency << '\n';
}
