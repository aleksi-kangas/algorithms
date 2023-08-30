#include <compare>
#include <cstdint>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <string>
#include <unordered_set>

enum class Direction { x, y };

struct FoldInstruction {
  Direction direction{Direction::x};
  std::int32_t value{0};
};

struct Point {
  std::int32_t x{0};
  std::int32_t y{0};
  auto operator<=>(const Point&) const = default;
};

template <>
struct std::hash<Point> {
  std::size_t operator()(const Point& p) const noexcept {
    const std::size_t h1 = std::hash<std::int32_t>{}(p.x);
    const std::size_t h2 = std::hash<std::int32_t>{}(p.y);
    return h1 ^ (h2 << 1);
  }
};

std::unordered_set<Point> ReadPoints() {
  std::unordered_set<Point> points{};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    const auto comma_index = line.find(',');
    Point point{
        .x = std::stoi(line.substr(0, comma_index)),
        .y = std::stoi(line.substr(comma_index + 1)),
    };
    points.insert(point);
  }
  return points;
}

std::vector<FoldInstruction> ReadFoldInstructions() {
  std::vector<FoldInstruction> instructions{};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    FoldInstruction instruction{};
    const auto equals_index = line.find('=');
    if (equals_index == std::string::npos)
      throw std::invalid_argument{"Invalid instruction"};
    switch (line[equals_index - 1]) {
      case 'x':
        instruction.direction = Direction::x;
        break;
      case 'y':
        instruction.direction = Direction::y;
        break;
      default:
        throw std::invalid_argument{"Invalid direction"};
    }
    instruction.value = std::stoi(line.substr(equals_index + 1));
    instructions.emplace_back(instruction);
  }
  return instructions;
}

void Fold(std::unordered_set<Point>& points, FoldInstruction instruction) {
  std::unordered_set<Point> new_points{};
  std::ranges::copy_if(points, std::inserter(new_points, new_points.end()), [&](const auto& point) {
    switch (instruction.direction) {
      case Direction::x:
        return point.x <= instruction.value;
      case Direction::y:
        return point.y <= instruction.value;
    }
  });
  for (const auto& point : points) {
    if (new_points.contains(point))
      continue;
    switch (instruction.direction) {
      case Direction::x:
        new_points.insert(Point{.x = instruction.value - (point.x - instruction.value), .y = point.y});
        break;
      case Direction::y:
        new_points.insert(Point{.x = point.x, .y = instruction.value - (point.y - instruction.value)});
        break;
    }
  }
  points = std::move(new_points);
}

void PrintPoints(const std::unordered_set<Point>& points) {
  const auto [min_x, max_x] = std::ranges::minmax_element(points, {}, &Point::x);
  const auto [min_y, max_y] = std::ranges::minmax_element(points, {}, &Point::y);
  for (std::int32_t y = min_y->y; y <= max_y->y; ++y) {
    for (std::int32_t x = min_x->x; x <= max_x->x; ++x) {
      std::cout << (points.contains(Point{.x = x, .y = y}) ? '#' : '.');
    }
    std::cout << '\n';
  }
}

int main() {
  auto points = ReadPoints();
  const auto instructions = ReadFoldInstructions();
  for (const auto& instruction : instructions) {
    Fold(points, instruction);
  }
  PrintPoints(points);
}
