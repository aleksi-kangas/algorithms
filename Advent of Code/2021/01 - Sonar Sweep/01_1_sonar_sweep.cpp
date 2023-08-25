#include <charconv>
#include <cstdint>
#include <iostream>
#include <string>

std::int32_t ParseInt32(std::string_view s) {
  std::int32_t result{0};
  auto [_, ec] = std::from_chars(s.data(), s.data() + s.size(), result);
  if (ec != std::errc{})
    throw std::runtime_error{"Failed to parse int32_t"};
  return result;
}

bool IsIncreasing(std::string_view previous, std::string_view current) {
  const std::int32_t previous_value = ParseInt32(previous);
  const std::int32_t current_value = ParseInt32(current);
  return previous_value < current_value;
}

int main() {
  std::int32_t answer{0};
  std::string previous{};
  std::string current{};
  while (std::getline(std::cin, current) && !current.empty()) {
    if (previous.empty()) {
      previous = current;
      continue;
    }

    if (IsIncreasing(previous, current)) {
      ++answer;
    }
    previous = current;
  }
  std::cout << answer << '\n';
}
