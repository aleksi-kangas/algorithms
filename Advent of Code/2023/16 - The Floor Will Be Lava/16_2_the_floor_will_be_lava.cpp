#include <cstdint>
#include <iostream>
#include <istream>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

using position_t = std::pair<std::int32_t, std::int32_t>;  // {row, column}

struct Beam {
  enum class Direction { kUp, kDown, kLeft, kRight };
  position_t position{};
  Direction direction{};

  bool operator==(const Beam& rhs) const noexcept { return position == rhs.position && direction == rhs.direction; }

  [[nodiscard]] position_t NextPosition() const;
};

template <>
struct std::hash<Beam> {
  std::size_t operator()(const Beam& b) const noexcept {
    const std::size_t h1 = std::hash<std::int32_t>{}(b.position.first);
    const std::size_t h2 = std::hash<std::int32_t>{}(b.position.second);
    const std::size_t h3 = std::hash<std::int32_t>{}(static_cast<std::int32_t>(b.direction));
    return h1 ^ (h2 << 1) ^ (h3 << 2);
  }
};

position_t Beam::NextPosition() const {
  switch (direction) {
    case Direction::kUp:
      return {position.first - 1, position.second};
    case Direction::kDown:
      return {position.first + 1, position.second};
    case Direction::kLeft:
      return {position.first, position.second - 1};
    case Direction::kRight:
      return {position.first, position.second + 1};
    default:
      throw std::logic_error{"Unhandled direction"};
  }
}

struct Contraption {
  enum class Tile { kEmpty, kMirrorForward, kMirrorBackward, kSplitterVertical, KSplitterHorizontal };
  std::vector<std::vector<Tile>> tiles{};

  [[nodiscard]] std::size_t EnergizedCount(Beam&& start_beam) const;

  [[nodiscard]] static Contraption Parse(std::istream& is);

 private:
  static void QueueNextBeams(const Beam& beam, Tile tile, std::queue<Beam>& q);
  static void QueueNextBeamMirrorForward(const Beam& beam, std::queue<Beam>& q);
  static void QueueNextBeamMirrorBackward(const Beam& beam, std::queue<Beam>& q);
  static void QueueNextBeamsSplitterVertical(const Beam& beam, std::queue<Beam>& q);
  static void QueueNextBeamsSplitterHorizontal(const Beam& beam, std::queue<Beam>& q);
};

std::size_t Contraption::EnergizedCount(Beam&& start_beam) const {
  std::unordered_set<Beam> cache{};
  std::vector energized(tiles.size(), std::vector(tiles[0].size(), false));
  std::queue<Beam> q{};
  q.emplace(std::move(start_beam));
  while (!q.empty()) {
    const auto beam = q.front();
    q.pop();
    if (cache.contains(beam))
      continue;
    if (beam.position.first >= 0 && beam.position.first < tiles.size() && beam.position.second >= 0 &&
        beam.position.second < tiles[0].size()) {
      energized[beam.position.first][beam.position.second] = true;
      cache.insert(beam);
    }
    const auto [r, c] = beam.NextPosition();
    if (r < 0 || r >= tiles.size() || c < 0 || c >= tiles[0].size())
      continue;
    QueueNextBeams(beam, tiles[r][c], q);
  }
  std::size_t count{0};
  for (const auto& row : energized) {
    for (const auto& tile : row) {
      if (tile)
        ++count;
    }
  }
  return count;
}

Contraption Contraption::Parse(std::istream& is) {
  Contraption contraption{};
  std::string line{};
  while (std::getline(is, line) && !line.empty()) {
    std::vector<Tile> row{};
    for (const auto c : line) {
      switch (c) {
        case '.':
          row.push_back(Tile::kEmpty);
          break;
        case '/':
          row.push_back(Tile::kMirrorForward);
          break;
        case '\\':
          row.push_back(Tile::kMirrorBackward);
          break;
        case '|':
          row.push_back(Tile::kSplitterVertical);
          break;
        case '-':
          row.push_back(Tile::KSplitterHorizontal);
          break;
        default:
          throw std::invalid_argument{"Invalid character in contraption"};
      }
    }
    contraption.tiles.push_back(std::move(row));
  }
  return contraption;
}

void Contraption::QueueNextBeams(const Beam& beam, Tile tile, std::queue<Beam>& q) {
  const auto [r, c] = beam.NextPosition();
  switch (tile) {
    case Tile::kEmpty: {
      q.emplace(Beam{.position = {r, c}, beam.direction});
    } break;
    case Tile::kMirrorForward: {
      QueueNextBeamMirrorForward(beam, q);
    } break;
    case Tile::kMirrorBackward: {
      QueueNextBeamMirrorBackward(beam, q);
    } break;
    case Tile::kSplitterVertical: {
      QueueNextBeamsSplitterVertical(beam, q);
    } break;
    case Tile::KSplitterHorizontal: {
      QueueNextBeamsSplitterHorizontal(beam, q);
    } break;
    default:
      throw std::logic_error{"Unhandled tile"};
  }
}

void Contraption::QueueNextBeamMirrorForward(const Beam& beam, std::queue<Beam>& q) {
  const auto [r, c] = beam.NextPosition();
  switch (beam.direction) {
    case Beam::Direction::kUp: {
      q.emplace(Beam{.position = {r, c}, .direction = Beam::Direction::kRight});
    } break;
    case Beam::Direction::kDown: {
      q.emplace(Beam{.position = {r, c}, .direction = Beam::Direction::kLeft});
    } break;
    case Beam::Direction::kLeft: {
      q.emplace(Beam{.position = {r, c}, .direction = Beam::Direction::kDown});
    } break;
    case Beam::Direction::kRight: {
      q.emplace(Beam{.position = {r, c}, .direction = Beam::Direction::kUp});
    } break;
    default:
      throw std::logic_error{"Unhandled direction"};
  }
}

void Contraption::QueueNextBeamMirrorBackward(const Beam& beam, std::queue<Beam>& q) {
  const auto [r, c] = beam.NextPosition();
  switch (beam.direction) {
    case Beam::Direction::kUp: {
      q.emplace(Beam{.position = {r, c}, .direction = Beam::Direction::kLeft});
    } break;
    case Beam::Direction::kDown: {
      q.emplace(Beam{.position = {r, c}, .direction = Beam::Direction::kRight});
    } break;
    case Beam::Direction::kLeft: {
      q.emplace(Beam{.position = {r, c}, .direction = Beam::Direction::kUp});
    } break;
    case Beam::Direction::kRight: {
      q.emplace(Beam{.position = {r, c}, .direction = Beam::Direction::kDown});
    } break;
    default:
      throw std::logic_error{"Unhandled direction"};
  }
}

void Contraption::QueueNextBeamsSplitterVertical(const Beam& beam, std::queue<Beam>& q) {
  const auto [r, c] = beam.NextPosition();
  switch (beam.direction) {
    case Beam::Direction::kUp:
    case Beam::Direction::kDown: {
      q.emplace(Beam{.position = {r, c}, .direction = beam.direction});
    } break;
    case Beam::Direction::kLeft:
    case Beam::Direction::kRight: {
      q.emplace(Beam{.position = {r, c}, .direction = Beam::Direction::kDown});
      q.emplace(Beam{.position = {r, c}, .direction = Beam::Direction::kUp});
    } break;
    default:
      throw std::logic_error{"Unhandled direction"};
  }
}

void Contraption::QueueNextBeamsSplitterHorizontal(const Beam& beam, std::queue<Beam>& q) {
  const auto [r, c] = beam.NextPosition();
  switch (beam.direction) {
    case Beam::Direction::kUp:
    case Beam::Direction::kDown: {
      q.emplace(Beam{.position = {r, c}, .direction = Beam::Direction::kRight});
      q.emplace(Beam{.position = {r, c}, .direction = Beam::Direction::kLeft});
    } break;
    case Beam::Direction::kLeft:
    case Beam::Direction::kRight: {
      q.emplace(Beam{.position = {r, c}, .direction = beam.direction});
    } break;
    default:
      throw std::logic_error{"Unhandled direction"};
  }
}

int main() {
  const auto contraption = Contraption::Parse(std::cin);
  std::size_t maximum_enegized_count{0};
  for (std::int32_t c = 0; c < contraption.tiles[0].size(); ++c) {
    maximum_enegized_count =
        std::max(maximum_enegized_count,
                 contraption.EnergizedCount(Beam{.position = {-1, c}, .direction = Beam::Direction::kDown}));
  }
  for (std::int32_t c = 0; c < contraption.tiles[0].size(); ++c) {
    maximum_enegized_count = std::max(
        maximum_enegized_count,
        contraption.EnergizedCount(Beam{.position = {contraption.tiles.size(), c}, .direction = Beam::Direction::kUp}));
  }
  for (std::int32_t r = 0; r < contraption.tiles.size(); ++r) {
    maximum_enegized_count =
        std::max(maximum_enegized_count,
                 contraption.EnergizedCount(Beam{.position = {r, -1}, .direction = Beam::Direction::kRight}));
  }
  for (std::int32_t r = 0; r < contraption.tiles.size(); ++r) {
    maximum_enegized_count =
        std::max(maximum_enegized_count, contraption.EnergizedCount(Beam{.position = {r, contraption.tiles[0].size()},
                                                                         .direction = Beam::Direction::kLeft}));
  }
  std::cout << maximum_enegized_count << std::endl;
}
