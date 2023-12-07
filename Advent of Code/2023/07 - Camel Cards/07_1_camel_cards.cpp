#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <istream>
#include <ranges>
#include <stdexcept>
#include <string>
#include <unordered_map>

class CamelCards {
 public:
  struct Hand {
    std::array<std::int32_t, 5> cards{};
    std::int32_t bid{};
    enum class Strength {
      HighCard,
      OnePair,
      TwoPair,
      ThreeOfAKind,
      FullHouse,
      FourOfAKind,
      FiveOfAKind,
    } strength{};

    bool operator<(const Hand& other) const;

    [[nodiscard]] bool IsXOfAKind(std::int32_t x) const;
    [[nodiscard]] bool IsFullHouse() const;
    [[nodiscard]] bool IsTwoPair() const;
    [[nodiscard]] bool IsPair() const;
    [[nodiscard]] std::int32_t HighCard() const;
    [[nodiscard]] Strength ComputeStrength() const;

    [[nodiscard]] static Hand Parse(std::string_view line);

   private:
    std::unordered_map<std::int32_t, std::int32_t> counts_{};

    Hand(const std::array<std::int32_t, 5>& cards, std::int32_t bid) : cards{cards}, bid{bid} {
      counts_.reserve(5);
      for (const auto& card : cards) {
        ++counts_[card];
      }
      strength = ComputeStrength();
    }
  };

  std::vector<Hand> sorted_hands{};

  [[nodiscard]] static CamelCards Parse(std::istream& is);

 private:
  explicit CamelCards(std::vector<Hand> hands) {
    std::sort(hands.begin(), hands.end());
    sorted_hands = std::move(hands);
  }
};

bool CamelCards::Hand::operator<(const Hand& other) const {
  if (strength != other.strength) {
    return strength < other.strength;
  }
  for (std::size_t i = 0; i < cards.size(); ++i) {
    if (cards[i] != other.cards[i]) {
      return cards[i] < other.cards[i];
    }
  }
  return false;
}

bool CamelCards::Hand::IsXOfAKind(std::int32_t x) const {
  return std::ranges::any_of(counts_, [x](const auto& pair) { return pair.second == x; });
}

bool CamelCards::Hand::IsFullHouse() const {
  return std::ranges::any_of(counts_, [](const auto& pair) { return pair.second == 3; }) &&
         std::ranges::any_of(counts_, [](const auto& pair) { return pair.second == 2; });
}

bool CamelCards::Hand::IsTwoPair() const {
  return std::ranges::count_if(counts_, [](const auto& pair) { return pair.second == 2; }) == 2;
}

bool CamelCards::Hand::IsPair() const {
  return std::ranges::count_if(counts_, [](const auto& pair) { return pair.second == 2; }) == 1;
}

std::int32_t CamelCards::Hand::HighCard() const {
  return std::ranges::max(cards);
}

CamelCards::Hand::Strength CamelCards::Hand::ComputeStrength() const {
  if (IsXOfAKind(5))
    return Strength::FiveOfAKind;
  if (IsXOfAKind(4))
    return Strength::FourOfAKind;
  if (IsFullHouse())
    return Strength::FullHouse;
  if (IsXOfAKind(3))
    return Strength::ThreeOfAKind;
  if (IsTwoPair())
    return Strength::TwoPair;
  if (IsPair())
    return Strength::OnePair;
  return Strength::HighCard;
}

CamelCards::Hand CamelCards::Hand::Parse(std::string_view line) {
  if (line.size() < 7) {
    throw std::invalid_argument{"Invalid line"};
  }
  std::array<std::int32_t, 5> cards{};
  for (std::size_t i = 0; i < 5; ++i) {
    switch (line[i]) {
      case 'A':
        cards[i] = 14;
        break;
      case 'K':
        cards[i] = 13;
        break;
      case 'Q':
        cards[i] = 12;
        break;
      case 'J':
        cards[i] = 11;
        break;
      case 'T':
        cards[i] = 10;
        break;
      case '9':
      case '8':
      case '7':
      case '6':
      case '5':
      case '4':
      case '3':
      case '2':
        cards[i] = line[i] - '0';
        break;
      default:
        throw std::invalid_argument{"Invalid card"};
    }
  }
  const std::int32_t bid = std::stoi(std::string(line.substr(6)));
  return {cards, bid};
}

CamelCards CamelCards::Parse(std::istream& is) {
  std::vector<Hand> hands{};
  std::string line{};
  while (std::getline(is, line) && !line.empty()) {
    hands.push_back(Hand::Parse(line));
  }
  return CamelCards{std::move(hands)};
}

int main() {
  const auto game = CamelCards::Parse(std::cin);
  std::int64_t winnings{};
  for (std::size_t i = 0; i < game.sorted_hands.size(); ++i) {
    const std::int64_t rank = static_cast<std::int64_t>(i) + 1;
    winnings += rank * game.sorted_hands[i].bid;
  }
  std::cout << winnings << std::endl;
}
