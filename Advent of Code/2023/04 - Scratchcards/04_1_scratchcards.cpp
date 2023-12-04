#include <cstdint>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

struct ScratchCard {
  std::int32_t id{0};
  std::unordered_set<std::int32_t> numbers{};
  std::unordered_set<std::int32_t> winning_numbers{};

  [[nodiscard]] std::int64_t Value() const {
    std::int64_t value{0};
    for (const auto number : numbers) {
      if (winning_numbers.contains(number)) {
        if (value == 0) {
          value = 1;
        } else {
          value *= 2;
        }
      }
    }
    return value;
  }

  [[nodiscard]] static ScratchCard Parse(std::string_view line) {
    ScratchCard card{};
    card.id = ParseId(line);
    const auto colon_position = line.find(':');
    if (colon_position == std::string_view::npos) {
      throw std::invalid_argument{"Invalid line: " + std::string{line}};
    }
    const auto vertical_bar_position = line.find('|', colon_position);
    if (vertical_bar_position == std::string_view::npos) {
      throw std::invalid_argument{"Invalid line: " + std::string{line}};
    }
    card.numbers = ParseNumbers(line.substr(vertical_bar_position + 1));
    card.winning_numbers = ParseNumbers(line.substr(colon_position + 1, vertical_bar_position - colon_position - 1));
    return card;
  }

 private:
  [[nodiscard]] static std::int32_t ParseId(std::string_view line) {
    const auto colon_position = line.find(':');
    if (colon_position == std::string_view::npos || colon_position < 5) {
      throw std::invalid_argument{"Invalid line: " + std::string{line}};
    }
    return std::stoi(std::string{line.substr(5, colon_position - 5)});
  }

  [[nodiscard]] static std::unordered_set<std::int32_t> ParseNumbers(std::string_view s) {
    std::stringstream ss{std::string{s}};
    std::string segment{};
    std::unordered_set<std::int32_t> numbers{};
    while (std::getline(ss, segment, ' ')) {
      if (!segment.empty()) {
        numbers.insert(std::stoi(segment));
      }
    }
    return numbers;
  }
};

int main() {
  std::vector<ScratchCard> cards{};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    cards.push_back(ScratchCard::Parse(line));
  }
  std::int64_t sum{0};
  for (const auto& card : cards) {
    sum += card.Value();
  }
  std::cout << sum << std::endl;
}
