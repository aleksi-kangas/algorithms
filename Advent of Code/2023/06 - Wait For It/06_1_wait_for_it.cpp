#include <chrono>
#include <cstdint>
#include <iostream>
#include <istream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

struct Race {
  std::chrono::milliseconds duration{0};
  std::int32_t distance{0};

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
};

std::vector<std::string> Split(const std::string& s, char delimiter) {
  std::vector<std::string> result{};
  std::stringstream ss{s};
  std::string segment{};
  while (std::getline(ss, segment, delimiter)) {
    if (!segment.empty()) {
      result.emplace_back(segment);
    }
  }
  return result;
}

std::vector<Race> ReadRaces(std::istream& is) {
  std::vector<Race> races{};
  std::string durations{};
  std::getline(is, durations);
  const auto durations_colon_position = durations.find(':');
  if (durations_colon_position == std::string::npos)
    throw std::runtime_error{"Invalid durations"};
  std::string distances{};
  std::getline(is, distances);
  const auto distances_colon_position = distances.find(':');
  if (distances_colon_position == std::string::npos)
    throw std::runtime_error{"Invalid distances"};
  const auto durations_segments = Split(durations.substr(durations_colon_position + 1), ' ');
  const auto distances_segments = Split(distances.substr(distances_colon_position + 1), ' ');
  if (durations_segments.size() != distances_segments.size())
    throw std::runtime_error{"Invalid durations and distances"};
  for (std::size_t i = 0; i < durations_segments.size(); ++i) {
    races.push_back(Race{.duration = std::chrono::milliseconds{std::stoi(durations_segments[i])},
                         .distance = std::stoi(distances_segments[i])});
  }
  return races;
}

int main() {
  const auto races = ReadRaces(std::cin);
  std::int64_t answer{1};
  for (const auto& r : races) {
    answer *= r.WaysToWin();
  }
  std::cout << answer << std::endl;
}
