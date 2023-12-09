#include <iostream>
#include <numeric>
#include <ranges>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

struct Sequence {
  std::vector<std::int32_t> values{};

  [[nodiscard]] bool AllZeros() const {
    return std::ranges::all_of(values, [](auto value) { return value == 0; });
  }

  [[nodiscard]] Sequence Differences() const {
    Sequence differences{};
    differences.values.reserve(values.size() - 1);
    for (std::size_t i = 1; i < values.size(); ++i) {
      differences.values.push_back(values[i] - values[i - 1]);
    }
    return differences;
  }

  static Sequence Parse(std::string_view line) {
    Sequence sequence{};
    std::istringstream iss{line.data()};
    std::string segment{};
    while (std::getline(iss, segment, ' ')) {
      sequence.values.push_back(std::stoi(segment));
    }
    return sequence;
  }
};

std::int32_t Extrapolate(std::stack<Sequence>& sequences) {
  std::int32_t extrapolated_value{0};
  while (!sequences.empty()) {
    auto [values] = sequences.top();
    sequences.pop();
    extrapolated_value = values.back() + extrapolated_value;
  }
  return extrapolated_value;
}

int main() {
  std::int32_t sum{0};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    std::stack<Sequence> sequences{};
    sequences.push(Sequence::Parse(line));
    while (true) {
      const auto sequence = sequences.top();
      const auto differences = sequence.Differences();
      sequences.push(differences);
      if (differences.AllZeros()) {
        sum += Extrapolate(sequences);
        break;
      }
    }
  }
  std::cout << sum << std::endl;
}
