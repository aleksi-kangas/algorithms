#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

enum class Move { kRock, kPaper, kScissors };

Move ParseMove(char c) {
  switch (c) {
    case 'A':
      return Move::kRock;
    case 'B':
      return Move::kPaper;
    case 'C':
      return Move::kScissors;
    case 'X':
      return Move::kRock;
    case 'Y':
      return Move::kPaper;
    case 'Z':
      return Move::kScissors;
    default:
      throw std::invalid_argument{"Invalid move"};
  }
}

std::int32_t Compare(Move m1, Move m2) {
  if (m1 == m2)
    return 0;
  if (m1 == Move::kRock)
    return m2 == Move::kScissors ? 1 : -1;
  if (m1 == Move::kPaper)
    return m2 == Move::kRock ? 1 : -1;
  if (m1 == Move::kScissors)
    return m2 == Move::kPaper ? 1 : -1;
  throw std::invalid_argument{"Invalid move"};
}

std::int32_t Score(Move m1, Move m2) {
  std::int32_t score{0};
  switch (m2) {
    case Move::kRock:
      score += 1;
      break;
    case Move::kPaper:
      score += 2;
      break;
    case Move::kScissors:
      score += 3;
      break;
  }
  const std::int32_t result = Compare(m2, m1);
  switch (result) {
    case 0:
      score += 3;
      break;
    case 1:
      score += 6;
      break;
    default:
      break;
  }
  return score;
}

int main() {
  std::int32_t total_score{0};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    if (line.size() != 3)
      throw std::invalid_argument{"Invalid input"};
    const Move m1 = ParseMove(line[0]);
    const Move m2 = ParseMove(line[2]);
    total_score += Score(m1, m2);
  }
  std::cout << total_score << '\n';
}
