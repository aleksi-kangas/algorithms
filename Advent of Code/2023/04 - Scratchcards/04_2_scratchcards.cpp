#include <cstdint>
#include <iostream>
#include <istream>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_set>

struct ScratchCard {
  std::int32_t id{0};
  std::unordered_set<std::int32_t> numbers{};
  std::unordered_set<std::int32_t> winning_numbers{};

  [[nodiscard]] std::int32_t WinningNumberCount() const {
    std::int32_t count{0};
    for (const auto number : numbers) {
      if (winning_numbers.contains(number)) {
        ++count;
      }
    }
    return count;
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

std::unordered_map<std::int32_t, ScratchCard> ReadScratchCards(std::istream& is) {
  std::unordered_map<std::int32_t, ScratchCard> cards{};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    auto card = ScratchCard::Parse(line);
    cards.insert({card.id, std::move(card)});
  }
  return cards;
}

int main() {
  const std::unordered_map<std::int32_t, ScratchCard> cards = ReadScratchCards(std::cin);
  std::queue<const ScratchCard*> scratch_cards{};
  for (const auto& [id, card] : cards) {
    scratch_cards.push(&card);
  }
  std::int64_t count{0};
  while (!scratch_cards.empty()) {
    const auto card = scratch_cards.front();
    scratch_cards.pop();
    ++count;
    const auto winning_number_count = card->WinningNumberCount();
    if (winning_number_count == 0) {
      continue;
    }
    for (std::int32_t i = card->id + 1; i <= card->id + winning_number_count; ++i) {
      const auto card_it = cards.find(i);
      if (card_it != cards.end()) {
        scratch_cards.push(&card_it->second);
      }
    }
  }
  std::cout << count << std::endl;
}
