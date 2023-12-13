#include <iostream>
#include <istream>
#include <optional>
#include <ranges>
#include <string>
#include <vector>

struct PatternNotes {
  enum class Cell { kAsh, kRock };

  std::vector<std::vector<Cell>> pattern{};

  [[nodiscard]] std::optional<std::size_t> FixAndFindHorizontalReflection() const;
  [[nodiscard]] std::optional<std::size_t> FixAndFindVerticalReflection() const;

  [[nodiscard]] static std::optional<PatternNotes> Parse(std::istream& is);
};

std::optional<std::size_t> PatternNotes::FixAndFindHorizontalReflection() const {
  if (pattern.empty() || pattern.front().empty() || pattern.size() == 1 || pattern.front().size() == 1)
    return std::nullopt;
  for (std::size_t row{1}; row < pattern.size(); ++row) {
    std::size_t difference_count{0};
    for (std::size_t column{0}; column < pattern.front().size(); ++column) {
      std::size_t l{row - 1};
      std::size_t r{row};
      while (l > 0 && r < pattern.size() - 1) {
        if (pattern[l][column] != pattern[r][column]) {
          ++difference_count;
        }
        --l;
        ++r;
      }
      if (pattern[l][column] != pattern[r][column]) {
        ++difference_count;
      }
    }
    if (difference_count == 1)
      return row;
  }
  return std::nullopt;
}

std::optional<std::size_t> PatternNotes::FixAndFindVerticalReflection() const {
  if (pattern.empty() || pattern.front().empty() || pattern.size() == 1 || pattern.front().size() == 1)
    return std::nullopt;
  for (std::size_t column{1}; column < pattern.front().size(); ++column) {
    std::size_t difference_count{0};
    for (std::size_t row{0}; row < pattern.size(); ++row) {
      std::size_t l{column - 1};
      std::size_t r{column};
      while (l > 0 && r < pattern.front().size() - 1) {
        if (pattern[row][l] != pattern[row][r]) {
          ++difference_count;
        }
        --l;
        ++r;
      }
      if (pattern[row][l] != pattern[row][r]) {
        ++difference_count;
      }
    }
    if (difference_count == 1)
      return column;
  }
  return std::nullopt;
}

std::optional<PatternNotes> PatternNotes::Parse(std::istream& is) {
  std::vector<std::vector<Cell>> pattern{};
  std::string line{};
  bool parsed{false};
  while (std::getline(is, line) && !line.empty()) {
    parsed = true;
    std::vector<Cell> row{};
    row.reserve(line.size());
    std::ranges::for_each(line, [&row](const char c) {
      switch (c) {
        case '.':
          row.push_back(Cell::kAsh);
          break;
        case '#':
          row.push_back(Cell::kRock);
          break;
        default:
          throw std::invalid_argument{"Invalid cell"};
      }
    });
    pattern.emplace_back(std::move(row));
  }
  return parsed ? std::make_optional(PatternNotes{.pattern = std::move(pattern)}) : std::nullopt;
}

int main() {
  std::size_t answer{0};
  while (true) {
    const auto pattern_notes = PatternNotes::Parse(std::cin);
    if (!pattern_notes.has_value())
      break;
    answer += pattern_notes->FixAndFindHorizontalReflection().value_or(0) * 100;
    answer += pattern_notes->FixAndFindVerticalReflection().value_or(0);
  }
  std::cout << answer << std::endl;
}
