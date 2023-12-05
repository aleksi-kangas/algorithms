#include <cstdint>
#include <iostream>
#include <istream>
#include <limits>
#include <optional>
#include <queue>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using Range = std::pair<std::uint64_t, std::uint64_t>;  // [start, end]

std::uint64_t Count(const Range& range) {
  return range.second - range.first + 1;
}

std::uint64_t End(std::uint64_t start, std::uint64_t count) {
  return start + count - 1;
}

struct MapRange {
  Range source{};
  Range destination{};

  struct Result {
    std::optional<Range> mapped_range{};    // As destination values
    std::vector<Range> remaining_ranges{};  // As source values
  };

  [[nodiscard]] MapRange::Result MapDestination(const Range& range) const;
  [[nodiscard]] static MapRange Parse(const std::string& line);
};

struct Map {
  std::vector<MapRange> map_ranges{};

  [[nodiscard]] std::vector<Range> MapDestination(const Range& range) const;
  [[nodiscard]] static Map Parse(std::istream& is, std::string& line);
};

MapRange::Result MapRange::MapDestination(const Range& range) const {
  // Case 1: No overlap
  if (range.second < source.first || source.second < range.first) {
    return {.mapped_range = {std::nullopt}, .remaining_ranges = {range}};
  }
  // Case 2: Fully contained
  if (source.first <= range.first && range.second <= source.second) {
    const auto index = range.first - source.first;
    const auto destination_start = destination.first + index;
    const auto destination_end = End(destination_start, Count(range));
    return {.mapped_range = Range{destination_start, destination_end}, .remaining_ranges = {}};
  }
  // Case 3: Partially contained
  if (range.first < source.first && source.first <= range.second && range.second <= source.second) {
    const auto remaining_range = Range{range.first, source.first - 1};
    const auto mapped_range = Range{destination.first, destination.first + Count(range) - Count(remaining_range) - 1};
    return {.mapped_range = mapped_range, .remaining_ranges = {remaining_range}};
  }
  if (source.first <= range.first && range.first <= source.second && source.second < range.second) {
    const auto remaining_range = Range{source.second + 1, range.second};
    const auto mapped_range = Range{destination.second - Count(range) + Count(remaining_range) + 1, destination.second};
    return {.mapped_range = mapped_range, .remaining_ranges = {remaining_range}};
  }
  if (range.first < source.first && source.second < range.second) {
    const auto remaining_range_l = Range{range.first, source.first - 1};
    const auto remaining_range_r = Range{source.second + 1, range.second};
    const auto mapped_range = Range{destination.first, destination.first + Count(range) - Count(remaining_range_l) - Count(remaining_range_r) - 1};
    return {.mapped_range = mapped_range, .remaining_ranges = {remaining_range_l, remaining_range_r}};
  }
  throw std::logic_error{"Unknown map case"};
}

MapRange MapRange::Parse(const std::string& line) {
  std::stringstream ss{line};
  std::string segment{};
  std::size_t index{0};
  std::uint64_t source_start{0};
  std::uint64_t destination_start{0};
  std::uint64_t count{0};
  while (std::getline(ss, segment, ' ')) {
    switch (index) {
      case 1:
        source_start = std::stoull(segment);
        break;
      case 0:
        destination_start = std::stoull(segment);
        break;
      case 2:
        count = std::stoull(segment);
        break;
      default:
        throw std::runtime_error{"Invalid map line"};
    }
    ++index;
  }
  return {.source = {source_start, source_start + count - 1},
          .destination = {destination_start, destination_start + count - 1}};
}

std::vector<Range> Map::MapDestination(const Range& range) const {
  std::vector<Range> destination_ranges{};
  std::queue<Range> q{};
  q.push(range);
  for (const auto& map_range : map_ranges) {
    if (q.empty())
      break;
    auto r = q.front();
    q.pop();
    const auto [mapped_range, remaining_ranges] = map_range.MapDestination(r);
    if (mapped_range.has_value()) {
      destination_ranges.push_back(mapped_range.value());
    }
    for (const auto& remaining_range : remaining_ranges) {
      q.push(remaining_range);
    }
  }
  while (!q.empty()) {
    destination_ranges.push_back(q.front());
    q.pop();
  }
  return destination_ranges;
}

Map Map::Parse(std::istream& is, std::string& line) {
  Map map{};
  map.map_ranges.push_back(MapRange::Parse(line));
  while (std::getline(is, line) && !line.empty()) {
    map.map_ranges.push_back(MapRange::Parse(line));
  }
  return map;
}

std::vector<Range> ReadSeeds(std::string_view line) {
  if (!line.starts_with("seeds: "))
    throw std::runtime_error{"Invalid seed line"};
  std::vector<Range> seed_ranges{};
  std::stringstream ss{std::string{line.substr(7)}};
  std::string segment{};
  std::uint64_t start{0};
  std::uint64_t count{0};
  bool reading_start{true};
  while (std::getline(ss, segment, ' ')) {
    if (reading_start) {
      start = std::stoull(segment);
    } else {
      count = std::stoull(segment);
      seed_ranges.emplace_back(start, start + count - 1);
    }
    reading_start = !reading_start;
  }
  return seed_ranges;
}

bool IgnoreLine(std::string_view line) {
  return line.empty() || line.ends_with("map:");
}

void SkipLines(std::istream& is, std::string& line) {
  while (std::getline(is, line) && IgnoreLine(line)) {}
}

std::vector<Range> ApplyMap(const std::vector<Range>& ranges, const Map& map) {
  std::vector<Range> destination_ranges{};
  for (const auto& range : ranges) {
    const auto mapped_ranges = map.MapDestination(range);
    destination_ranges.insert(destination_ranges.end(), mapped_ranges.begin(), mapped_ranges.end());
  }
  return destination_ranges;
}

std::uint64_t Minimum(const std::vector<Range>& ranges) {
  std::uint64_t minimum{std::numeric_limits<std::uint64_t>::max()};
  for (const auto& [start, _] : ranges) {
    minimum = std::min(minimum, start);
  }
  return minimum;
}

int main() {
  std::string line{};
  std::getline(std::cin, line);
  auto ranges = ReadSeeds(line);
  SkipLines(std::cin, line);
  const auto seed_to_soil_map = Map::Parse(std::cin, line);
  SkipLines(std::cin, line);
  const auto soil_to_fertilizer_map = Map::Parse(std::cin, line);
  SkipLines(std::cin, line);
  const auto fertilizer_to_water_map = Map::Parse(std::cin, line);
  SkipLines(std::cin, line);
  const auto water_to_light_map = Map::Parse(std::cin, line);
  SkipLines(std::cin, line);
  const auto light_to_temperature_map = Map::Parse(std::cin, line);
  SkipLines(std::cin, line);
  const auto temperature_to_humidity_map = Map::Parse(std::cin, line);
  SkipLines(std::cin, line);
  const auto humidity_to_location_map = Map::Parse(std::cin, line);

  ranges = ApplyMap(ranges, seed_to_soil_map);
  ranges = ApplyMap(ranges, soil_to_fertilizer_map);
  ranges = ApplyMap(ranges, fertilizer_to_water_map);
  ranges = ApplyMap(ranges, water_to_light_map);
  ranges = ApplyMap(ranges, light_to_temperature_map);
  ranges = ApplyMap(ranges, temperature_to_humidity_map);
  ranges = ApplyMap(ranges, humidity_to_location_map);

  std::cout << Minimum(ranges) << std::endl;
}
