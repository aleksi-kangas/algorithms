#include <array>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>

std::vector<std::int32_t> ReadFishTimers() {
  std::vector<std::int32_t> fish_timers{};
  std::string line{};
  std::cin >> line;
  std::stringstream ss{line};
  for (std::int32_t fish_timer; ss >> fish_timer;) {
    fish_timers.emplace_back(fish_timer);
    if (ss.peek() == ',') {
      ss.ignore();
    }
  }
  return fish_timers;
}

int main() {
  const auto initial_timers = ReadFishTimers();
  std::array<std::int64_t, 9> timers{};
  for (int fish_timer : initial_timers) {
    ++timers[fish_timer];
  }
  for (std::size_t d = 1; d <= 256; ++d) {
    const std::int64_t new_fish_count = timers[0];
    for (std::size_t i = 0; i < timers.size() - 1; ++i) {
      timers[i] = timers[i + 1];
    }
    timers[6] += new_fish_count;
    timers[8] = new_fish_count;
  }
  std::int64_t answer{0};
  for (std::int64_t timer : timers) {
    answer += timer;
  }
  std::cout << answer << std::endl;
}
