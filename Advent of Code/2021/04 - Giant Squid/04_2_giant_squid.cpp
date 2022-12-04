#include <algorithm>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

using ll = long long;

class Board {
 public:
  Board() = default;

  [[nodiscard]] bool CheckWin() const {
    bool row_win = std::any_of(rows_.begin(), rows_.end(), [](const auto& row) { return row.empty(); });
    bool column_win = std::any_of(columns_.begin(), columns_.end(), [](const auto& column) { return column.empty(); });
    return row_win || column_win;
  }

  [[nodiscard]] int ComputeScore() const {
    int score = 0;
    for (auto& row : rows_) {
      for (int n : row) {
        score += n;
      }
    }
    return score;
  }

  void MarkNumber(int number) {
    for (auto& row : rows_) {
      row.erase(number);
    }
    for (auto& column : columns_) {
      column.erase(number);
    }
  }

  void ParseRow(const string& line) {
    vector<int> numbers;
    stringstream ss(line);
    for (int i; ss >> i;) {
      numbers.push_back(i);
      if (ss.peek() == ' ') {
        ss.ignore();
      }
    }
    set<int> row;
    for (int i = 0; i < 5; ++i) {
      row.insert(numbers[i]);
      if (columns_.size() == i) {
        columns_.emplace_back();
      }
      columns_[i].insert(numbers[i]);
    }
    rows_.push_back(move(row));
  }

 private:
  vector<set<int>> rows_;
  vector<set<int>> columns_;
  bool win = false;
};

vector<int> ParseNumbers() {
  string s;
  cin >> s;
  vector<int> numbers;

  stringstream ss(s);
  for (int i; ss >> i;) {
    numbers.push_back(i);
    if (ss.peek() == ',') {
      ss.ignore();
    }
  }
  cin.ignore();
  return numbers;
}

vector<Board> ParseBoards() {
  vector<Board> boards;
  string s;
  int successive_empty_lines = 0;
  while (getline(cin, s)) {
    if (s.empty()) {
      ++successive_empty_lines;
      if (successive_empty_lines == 2) {
        break;
      }
      continue;
    } else {
      successive_empty_lines = 0;
      boards.emplace_back();
      Board& board = boards.back();
      board.ParseRow(s);
      for (int i = 0; i < 4; ++i) {  // Read rest of the rows of the board.
        getline(cin, s);
        board.ParseRow(s);
      }
    }
  }
  return boards;
}

int Solve() {
  vector<int> numbers = ParseNumbers();
  vector<Board> boards = ParseBoards();

  int nr_boards = static_cast<int>(boards.size());
  int boards_left = static_cast<int>(boards.size());
  vector<bool> board_won(nr_boards, false);
  for (int number : numbers) {
    for (int b = 0; b < nr_boards; ++b) {
      auto& board = boards[b];
      if (board_won[b]) {
        continue;
      }
      board.MarkNumber(number);
      if (board.CheckWin()) {
        board_won[b] = true;
        --boards_left;
        if (boards_left == 0) {
          return board.ComputeScore() * number;
        }
      }
    }
  }
  return 0;
}

int main() {
  int score = Solve();
  cout << score << endl;
}
