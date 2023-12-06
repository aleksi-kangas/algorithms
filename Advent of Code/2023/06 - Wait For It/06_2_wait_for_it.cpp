#include <chrono>
#include <cstdint>
#include <iostream>
#include <istream>
#include <stdexcept>
#include <string>

struct Race {
  std::chrono::milliseconds duration{0};
  std::int64_t distance{0};

  [[nodiscard]] std::int64_t WaysToWin() const {
    std::int64_t count{0};
    for (std::int64_t charge_time = 0; charge_time <= duration.count(); ++charge_time) {
      const std::int64_t remaining_time = duration.count() - charge_time;
      const std::int64_t speed = charge_time;
      const std::int64_t distance_covered = speed * remaining_time;
      if (distance_covered > distance) {
        ++count;
      }
    }
    return count;
  }

  [[nodiscard]] static Race Parse(std::istream& is) {
    std::string line{};
    std::getline(is, line);
    const auto duration_colon_position = line.find(':');
    if (duration_colon_position == std::string::npos)
      throw std::runtime_error{"Invalid duration"};
    std::string duration_string{};
    duration_string.reserve(line.size() - duration_colon_position);
    for (std::size_t i = duration_colon_position + 1; i < line.size(); ++i) {
      if (std::isdigit(line[i])) {
        duration_string.push_back(line[i]);
      }
    }
    const auto duration = std::chrono::milliseconds{std::stol(duration_string)};
    std::getline(is, line);
    const auto distance_colon_position = line.find(':');
    if (distance_colon_position == std::string::npos)
      throw std::runtime_error{"Invalid distance"};
    std::string distance_string{};
    distance_string.reserve(line.size() - distance_colon_position);
    for (std::size_t i = distance_colon_position + 1; i < line.size(); ++i) {
      if (std::isdigit(line[i])) {
        distance_string.push_back(line[i]);
      }
    }
    const auto distance = std::stol(distance_string);
    return {.duration = duration, .distance = distance};
  }
};

int main() {
  const auto race = Race::Parse(std::cin);
  std::cout << race.WaysToWin() << std::endl;
}
