#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

enum class Move { Rock, Paper, Scissors };
enum class Strategy { kLoss, kDraw, kWin };

Move ParseMove(char c) {
  switch (c) {
    case 'A':
      return Move::Rock;
    case 'B':
      return Move::Paper;
    case 'C':
      return Move::Scissors;
    default:
      throw std::invalid_argument{"Invalid move"};
  }
}

Strategy ParseStrategy(char c) {
  switch (c) {
    case 'X':
      return Strategy::kLoss;
    case 'Y':
      return Strategy::kDraw;
    case 'Z':
      return Strategy::kWin;
    default:
      throw std::invalid_argument{"Invalid strategy"};
  }
}

Move ExecuteStrategy(Move opponent, Strategy strategy) {
  switch (strategy) {
    case Strategy::kDraw:
      return opponent;
    case Strategy::kLoss: {
      switch (opponent) {
        case Move::Rock:
          return Move::Scissors;
        case Move::Paper:
          return Move::Rock;
        case Move::Scissors:
          return Move::Paper;
      }
    }
    case Strategy::kWin: {
      switch (opponent) {
        case Move::Rock:
          return Move::Paper;
        case Move::Paper:
          return Move::Scissors;
        case Move::Scissors:
          return Move::Rock;
      }
    }
  }
}

std::int32_t Score(Move opponent, Strategy strategy) {
  std::int32_t score{0};
  const Move move = ExecuteStrategy(opponent, strategy);
  switch (move) {
    case Move::Rock:
      score += 1;
      break;
    case Move::Paper:
      score += 2;
      break;
    case Move::Scissors:
      score += 3;
      break;
  }
  switch (strategy) {
    case Strategy::kDraw:
      score += 3;
      break;
    case Strategy::kWin:
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
    const Move opponent = ParseMove(line[0]);
    const Strategy strategy = ParseStrategy(line[2]);
    total_score += Score(opponent, strategy);
  }
  std::cout << total_score << '\n';
}
