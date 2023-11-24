#include <array>
#include <cstdint>
#include <functional>
#include <iostream>
#include <istream>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>

struct Monkey {
  std::queue<std::int32_t> items{};
  std::function<std::int32_t(std::int32_t)> inspection_operation{};
  std::function<std::size_t(std::int32_t)> check_operation{};

  [[nodiscard]] static Monkey Parse(const std::vector<std::string>& lines) {
    if (lines.size() < 6) {
      throw std::runtime_error{"Invalid monkey"};
    }
    Monkey monkey{};
    monkey.items = ParseItems(lines[1]);
    monkey.inspection_operation = ParseInspectionOperation(lines[2]);
    monkey.check_operation = ParseCheckOperation(lines[3], lines[4], lines[5]);
    return monkey;
  }

 private:
  [[nodiscard]] static std::queue<std::int32_t> ParseItems(std::string_view s) {
    const auto colon_position = s.find(':');
    if (colon_position == std::string_view::npos) {
      throw std::runtime_error{"Invalid items"};
    }
    std::queue<std::int32_t> items{};
    std::istringstream iss{std::string{s.substr(colon_position + 1)}};
    std::string item{};
    while (std::getline(iss, item, ',')) {
      items.push(std::stoi(item));
    }
    return items;
  }

  [[nodiscard]] static std::function<std::int32_t(std::int32_t)> ParseInspectionOperation(std::string_view s) {
    const auto old_position = s.find("old");
    if (old_position == std::string_view::npos) {
      throw std::runtime_error{"Invalid inspection operation"};
    }
    const char operation = s[old_position + 4];
    const std::optional<std::int32_t> operand = [&s, old_position]() -> std::optional<std::int32_t> {
      try {
        return std::stoi(std::string{s.substr(old_position + 5)});
      } catch (...) {
        return std::nullopt;
      }
    }();
    return [operation, operand](std::int32_t value) {
      switch (operation) {
        case '+':
          return value + operand.value_or(value);
        case '-':
          return value - operand.value_or(value);
        case '*':
          return value * operand.value_or(value);
        case '/':
          return value / operand.value_or(value);
        default:
          throw std::runtime_error{"Invalid inspection operation"};
      }
    };
  }

  [[nodiscard]] static std::function<std::size_t(std::int32_t)> ParseCheckOperation(std::string_view s1,
                                                                                    std::string_view s2,
                                                                                    std::string_view s3) {
    const auto divisible_by_location = s1.find("divisible by");
    if (divisible_by_location == std::string_view::npos) {
      throw std::runtime_error{"Invalid check operation"};
    }
    const auto divider = std::stoi(std::string{s1.substr(divisible_by_location + 13)});
    const auto true_monkey_position = s2.find("monkey");
    if (true_monkey_position == std::string_view::npos) {
      throw std::runtime_error{"Invalid check operation"};
    }
    const auto true_monkey = std::stoi(std::string{s2.substr(true_monkey_position + 6)});
    const auto false_monkey_position = s3.find("monkey");
    if (false_monkey_position == std::string_view::npos) {
      throw std::runtime_error{"Invalid check operation"};
    }
    const auto false_monkey = std::stoi(std::string{s3.substr(false_monkey_position + 6)});
    return [divider, true_monkey, false_monkey](std::int32_t value) {
      return value % divider == 0 ? true_monkey : false_monkey;
    };
  }
};

int main() {
  std::vector<std::string> monkey_lines{};
  std::string line{};
  std::vector<Monkey> monkeys{};
  std::size_t empty_line_count{0};
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      if (empty_line_count == 0) {
        monkeys.push_back(Monkey::Parse(monkey_lines));
      }
      monkey_lines.clear();
      ++empty_line_count;
      if (empty_line_count == 2) {
        break;
      }
    } else {
      monkey_lines.push_back(line);
      empty_line_count = 0;
    }
  }

  std::vector<std::size_t> inspection_counts(monkeys.size(), 0);
  for (std::size_t r = 1; r <= 20; ++r) {
    for (std::size_t monkey_index = 0; monkey_index < monkeys.size(); ++monkey_index) {
      Monkey& monkey = monkeys[monkey_index];
      while (!monkey.items.empty()) {
        std::int32_t worry_level = monkey.items.front();
        monkey.items.pop();
        worry_level = monkey.inspection_operation(worry_level);
        worry_level /= 3;
        const std::size_t target_monkey_index = monkey.check_operation(worry_level);
        ++inspection_counts[monkey_index];
        monkeys[target_monkey_index].items.push(worry_level);
      }
    }
  }

  std::sort(inspection_counts.begin(), inspection_counts.end());
  std::cout << inspection_counts[inspection_counts.size() - 1] * inspection_counts[inspection_counts.size() - 2]
            << '\n';
}
