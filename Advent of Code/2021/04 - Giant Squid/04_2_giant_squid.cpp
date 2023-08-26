#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

class Board {
 public:
  static constexpr std::int32_t kSize{5};

  static Board ParseBoard() {
    Board board{};
    std::string line{};
    std::size_t row_index{0};
    while (std::getline(std::cin, line) && !line.empty()) {
      std::stringstream ss{line};
      std::size_t column_index{0};
      for (std::int32_t draw; ss >> draw;) {
        board.rows_[row_index].insert(draw);
        board.columns_[column_index++].insert(draw);
        if (ss.peek() == ' ') {
          ss.ignore();
        }
      }
      ++row_index;
    }
    return board;
  }

  void MarkDraw(std::int32_t draw) {
    std::ranges::for_each(rows_, [&](auto& row) { row.erase(draw); });
    std::ranges::for_each(columns_, [&](auto& column) { column.erase(draw); });
  }

  [[nodiscard]] bool CheckWin() const {
    return std::ranges::any_of(rows_, [](const auto& row) { return row.empty(); }) ||
           std::ranges::any_of(columns_, [](const auto& column) { return column.empty(); });
  }

  [[nodiscard]] std::int32_t ComputeScore() const {
    std::int32_t sum{0};
    std::ranges::for_each(rows_, [&](const auto& row) { sum += std::accumulate(row.begin(), row.end(), 0); });
    return sum;
  }

 private:
  std::array<std::unordered_set<std::int32_t>, kSize> rows_{};
  std::array<std::unordered_set<std::int32_t>, kSize> columns_{};
};

std::vector<std::int32_t> ReadDraws() {
  std::vector<std::int32_t> draws{};
  std::string draw_line{};
  std::getline(std::cin, draw_line);
  std::stringstream ss{draw_line};
  for (std::int32_t draw; ss >> draw;) {
    draws.emplace_back(draw);
    if (ss.peek() == ',') {
      ss.ignore();
    }
  }
  return draws;
}

std::vector<Board> ReadBoards() {
  std::int32_t empty_line_count{0};
  std::string board_line{};
  std::vector<Board> boards{};
  while (empty_line_count < 2) {
    if (std::cin.peek() == '\n') {
      std::cin.ignore();
      ++empty_line_count;
      continue;
    } else {
      empty_line_count = 0;
      boards.emplace_back(Board::ParseBoard());
    }
  }
  return boards;
}

int main() {
  const auto draws = ReadDraws();
  auto boards = ReadBoards();
  std::vector<bool> board_won(boards.size(), false);
  for (const auto& draw : draws) {
    for (std::size_t board_index = 0; board_index < boards.size(); ++board_index) {
      if (board_won[board_index])
        continue;
      auto& board = boards[board_index];
      board.MarkDraw(draw);
      if (board.CheckWin()) {
        board_won[board_index] = true;
        if (std::ranges::all_of(board_won, [](const auto& won) { return won; })) {
          std::cout << board.ComputeScore() * draw << std::endl;
          return 0;
        }
      }
    }
  }
  return -1;
}
