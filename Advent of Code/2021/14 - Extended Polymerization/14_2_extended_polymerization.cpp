#include <cstdint>
#include <iostream>
#include <limits>
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

std::unordered_map<std::pair<char, char>, std::int64_t> ReadInitialFrequencies() {
  std::unordered_map<std::pair<char, char>, std::int64_t> frequencies{};
  std::string line{};
  std::getline(std::cin, line);
  for (std::size_t i = 1; i < line.size(); ++i) {
    ++frequencies[{line[i - 1], line[i]}];
  }
  return frequencies;
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

void InsertionStep(std::unordered_map<std::pair<char, char>, std::int64_t>& frequencies,
                   const std::unordered_map<std::pair<char, char>, char>& rules) {
  std::unordered_map<std::pair<char, char>, std::int64_t> new_frequencies{};
  for (const auto& [pair, frequency] : frequencies) {
    if (const auto it = rules.find(pair); it != rules.end()) {
      new_frequencies[{pair.first, it->second}] += frequency;
      new_frequencies[{it->second, pair.second}] += frequency;
    } else {
      new_frequencies[pair] = frequency;
    }
  }
  frequencies = std::move(new_frequencies);
}

std::pair<std::int64_t, std::int64_t> MinMaxFrequency(
    const std::unordered_map<std::pair<char, char>, std::int64_t>& frequencies) {
  std::unordered_map<char, std::int64_t> frequency{};
  for (const auto& [pair, f] : frequencies) {
    frequency[pair.first] += f;
    frequency[pair.second] += f;
  }
  std::int64_t min_frequency{std::numeric_limits<std::int64_t>::max()};
  std::int64_t max_frequency{std::numeric_limits<std::int64_t>::min()};
  for (const auto [_, f] : frequency) {
    min_frequency = std::min(min_frequency, (f + 1) / 2);
    max_frequency = std::max(max_frequency, (f + 1) / 2);
  }
  return {min_frequency, max_frequency};
}

int main() {
  auto frequencies = ReadInitialFrequencies();
  std::cin.ignore();
  const auto rules = ReadInsertionRules();
  for (std::size_t step = 1; step <= 40; ++step) {
    InsertionStep(frequencies, rules);
  }
  const auto [min_frequency, max_frequency] = MinMaxFrequency(frequencies);
  std::cout << max_frequency - min_frequency << '\n';
}
