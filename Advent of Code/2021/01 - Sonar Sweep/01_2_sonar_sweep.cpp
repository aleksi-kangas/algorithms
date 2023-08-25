#include <charconv>
#include <cstdint>
#include <iostream>
#include <queue>
#include <string>

std::int32_t ParseInt32(std::string_view s) {
  std::int32_t result{0};
  auto [_, ec] = std::from_chars(s.data(), s.data() + s.size(), result);
  if (ec != std::errc{})
    throw std::runtime_error{"Failed to parse int32_t"};
  return result;
}

int main() {
  constexpr std::size_t kWindowSize = 3;
  std::int32_t answer{0};
  std::queue<std::int32_t> q{};
  std::int32_t sum{0};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    const std::int32_t value = ParseInt32(line);
    if (q.size() == kWindowSize) {
      const std::int32_t popped = q.front();
      q.pop();
      const std::int32_t previous_sum = sum;
      sum -= popped;
      q.push(value);
      sum += value;
      if (previous_sum < sum) {
        ++answer;
      }
    } else {
      q.push(value);
      sum += value;
    }
  }
  std::cout << answer << '\n';
}
