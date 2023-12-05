#include <cstdint>
#include <iostream>
#include <istream>
#include <optional>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
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

  [[nodiscard]] std::optional<std::uint64_t> MapDestination(std::uint64_t value) const;
  [[nodiscard]] static MapRange Parse(const std::string& line);
};

struct Map {
  std::vector<MapRange> map_ranges{};

  [[nodiscard]] std::uint64_t MapDestination(std::uint64_t value) const;
  [[nodiscard]] static Map Parse(std::istream& is, std::string& line);
};

std::optional<std::uint64_t> MapRange::MapDestination(std::uint64_t value) const {
  if (value < source.first || source.second < value)
    return std::nullopt;
  return destination.first + (value - source.first);
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

std::uint64_t Map::MapDestination(std::uint64_t value) const {
  // Assume disjoint ranges
  for (const auto& range : map_ranges) {
    const auto destination = range.MapDestination(value);
    if (destination.has_value())
      return destination.value();
  }
  return value;
}

Map Map::Parse(std::istream& is, std::string& line) {
  Map map{};
  map.map_ranges.push_back(MapRange::Parse(line));
  while (std::getline(is, line) && !line.empty()) {
    map.map_ranges.push_back(MapRange::Parse(line));
  }
  return map;
}

std::vector<std::uint64_t> ReadSeeds(std::string_view line) {
  if (!line.starts_with("seeds: "))
    throw std::runtime_error{"Invalid seed line"};
  std::vector<std::uint64_t> seeds{};
  std::stringstream ss{std::string{line.substr(7)}};
  std::string segment{};
  while (std::getline(ss, segment, ' ')) {
    seeds.push_back(std::stoull(segment));
  }
  return seeds;
}

bool IgnoreLine(std::string_view line) {
  return line.empty() || line.ends_with("map:");
}

void SkipLines(std::istream& is, std::string& line) {
  while (std::getline(is, line) && IgnoreLine(line)) {}
}

void ApplyMap(std::vector<std::uint64_t>& values, const Map& map) {
  for (auto& value : values) {
    value = map.MapDestination(value);
  }
}

int main() {
  std::string line{};
  std::getline(std::cin, line);
  auto values = ReadSeeds(line);
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

  ApplyMap(values, seed_to_soil_map);
  ApplyMap(values, soil_to_fertilizer_map);
  ApplyMap(values, fertilizer_to_water_map);
  ApplyMap(values, water_to_light_map);
  ApplyMap(values, light_to_temperature_map);
  ApplyMap(values, temperature_to_humidity_map);
  ApplyMap(values, humidity_to_location_map);

  std::cout << *std::ranges::min_element(values) << std::endl;
}
