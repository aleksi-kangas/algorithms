#include <iostream>
#include <istream>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

using position_t = std::pair<std::size_t, std::size_t>;  // {row, column}

template <>
struct std::hash<position_t> {
  std::size_t operator()(const position_t& p) const noexcept {
    const std::size_t h1 = std::hash<std::size_t>{}(p.first);
    const std::size_t h2 = std::hash<std::size_t>{}(p.second);
    return h1 ^ (h2 << 1);
  }
};

enum class Tile { kGardenPlot, kRock };

struct Map {
  std::vector<std::vector<Tile>> tiles{};
  position_t start{};

  [[nodiscard]] std::size_t PlotsReachedAt(std::size_t steps) const;

  [[nodiscard]] static Map Parse(std::istream& is);

 private:
  struct Step {
    position_t position{};
    std::size_t steps{};
  };
};

std::size_t Map::PlotsReachedAt(std::size_t steps) const {
  std::queue<Step> q{};
  q.push({start, 0});
  for (std::size_t s{1}; s <= steps; ++s) {
    std::queue<Step> next{};
    std::unordered_set<position_t> visited{};
    while (!q.empty()) {
      const auto step = q.front();
      q.pop();
      if (step.position.first > 0) {
        const position_t up{step.position.first - 1, step.position.second};
        if (tiles[up.first][up.second] == Tile::kGardenPlot && !visited.contains(up)) {
          next.push({up, step.steps + 1});
          visited.insert(up);
        }
      }
      if (step.position.second > 0) {
        const position_t left{step.position.first, step.position.second - 1};
        if (tiles[left.first][left.second] == Tile::kGardenPlot && !visited.contains(left)) {
          next.push({left, step.steps + 1});
          visited.insert(left);
        }
      }
      if (step.position.first < tiles.size() - 1) {
        const position_t down{step.position.first + 1, step.position.second};
        if (tiles[down.first][down.second] == Tile::kGardenPlot && !visited.contains(down)) {
          next.push({down, step.steps + 1});
          visited.insert(down);
        }
      }
      if (step.position.second < tiles[step.position.first].size() - 1) {
        const position_t right{step.position.first, step.position.second + 1};
        if (tiles[right.first][right.second] == Tile::kGardenPlot && !visited.contains(right)) {
          next.push({right, step.steps + 1});
          visited.insert(right);
        }
      }
    }
    q = std::move(next);
  }
  return q.size();
}

Map Map::Parse(std::istream& is) {
  Map map{};
  std::string line{};
  while (std::getline(is, line) && !line.empty()) {
    std::vector<Tile> row{};
    row.reserve(line.size());
    for (const auto c : line) {
      switch (c) {
        case '.': {
          row.push_back(Tile::kGardenPlot);
        } break;
        case '#': {
          row.push_back(Tile::kRock);
        } break;
        case 'S': {
          map.start = {map.tiles.size(), row.size()};
          row.push_back(Tile::kGardenPlot);
        } break;
        default:
          throw std::invalid_argument{"Invalid tile"};
      }
    }
    map.tiles.push_back(std::move(row));
  }
  return map;
}

int main() {
  const auto map = Map::Parse(std::cin);
  std::cout << map.PlotsReachedAt(64) << std::endl;
}
