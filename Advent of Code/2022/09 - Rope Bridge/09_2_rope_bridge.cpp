#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <utility>

enum class Direction { kLeft, kRight, kUp, kDown };

struct Point {
  std::int32_t x{0};
  std::int32_t y{0};
  auto operator<=>(const Point&) const = default;
};

using position_t = std::pair<std::int32_t, std::int32_t>;

template <>
struct std::hash<Point> {
  std::size_t operator()(const Point& p) const noexcept {
    const std::size_t h1 = std::hash<std::int32_t>{}(p.x);
    const std::size_t h2 = std::hash<std::int32_t>{}(p.y);
    return h1 ^ (h2 << 1);
  }
};

std::pair<Direction, std::int32_t> ParseMotion(std::string_view s) {
  Direction d{Direction::kLeft};
  switch (s[0]) {
    case 'L':
      d = Direction::kLeft;
      break;
    case 'R':
      d = Direction::kRight;
      break;
    case 'U':
      d = Direction::kUp;
      break;
    case 'D':
      d = Direction::kDown;
      break;
    default:
      throw std::runtime_error{"Invalid direction"};
  }
  const std::int32_t distance = std::stoi(std::string{s.substr(1)});
  return {d, distance};
}

void MoveHead(Point& head, Direction direction) {
  switch (direction) {
    case Direction::kLeft:
      --head.x;
      break;
    case Direction::kRight:
      ++head.x;
      break;
    case Direction::kUp:
      ++head.y;
      break;
    case Direction::kDown:
      --head.y;
      break;
    default:
      throw std::runtime_error{"Invalid direction"};
  }
}

void MoveTail(const Point& head, Point& tail) {
  const std::int32_t dx = head.x - tail.x;
  const std::int32_t dy = head.y - tail.y;
  if (dx == 0 || dy == 0) {
    if (std::abs(dx) >= 2) {
      tail.x += (dx > 0) ? 1 : -1;
    }
    if (std::abs(dy) >= 2) {
      tail.y += (dy > 0) ? 1 : -1;
    }
  } else if (std::abs(dx) != 1 || std::abs(dy) != 1) {
    tail.x += (dx > 0) ? 1 : -1;
    tail.y += (dy > 0) ? 1 : -1;
  }
}

int main() {
  std::array<Point, 10> rope{};
  std::unordered_set<Point> tail_visited{};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    const auto [d, distance] = ParseMotion(line);
    for (std::int32_t i = 0; i < distance; ++i) {
      MoveHead(rope[0], d);
      for (std::size_t j = 1; j < rope.size(); ++j) {
        MoveTail(rope[j - 1], rope[j]);
      }
      tail_visited.insert(rope[rope.size() - 1]);
    }
  }
  std::cout << tail_visited.size() << '\n';
}
