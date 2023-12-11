#include <cstdint>
#include <iostream>
#include <optional>
#include <queue>
#include <ranges>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

struct Graph {
  enum class Pipe { kStart, kVertical, kHorizontal, kUpRight, kUpLeft, kDownRight, kDownLeft };

  struct Vertex {
    std::pair<std::size_t, std::size_t> position{};
    std::optional<Pipe> pipe{std::nullopt};
    std::int32_t distance{-1};

    [[nodiscard]] static bool AreConnected(const Vertex& lhs, const Vertex& rhs);
  };

  std::vector<std::vector<Vertex>> vertices{};
  std::pair<std::size_t, std::size_t> start{};

  std::int32_t BFSMaxDistance();

  [[nodiscard]] static Graph Parse(std::istream& is);
};

bool Graph::Vertex::AreConnected(const Vertex& lhs, const Vertex& rhs) {
  if (!lhs.pipe.has_value() || !rhs.pipe.has_value())
    return false;
  if (lhs.position.first != rhs.position.first && lhs.position.second != rhs.position.second)
    return false;
  const bool is_rhs_above = lhs.position.first > rhs.position.first;
  const bool is_rhs_below = lhs.position.first < rhs.position.first;
  const bool is_rhs_left = lhs.position.second > rhs.position.second;
  const bool is_rhs_right = lhs.position.second < rhs.position.second;
  if (lhs.pipe == Pipe::kStart) {
    if (is_rhs_above)
      return rhs.pipe == Pipe::kVertical || rhs.pipe == Pipe::kDownRight || rhs.pipe == Pipe::kDownLeft;
    if (is_rhs_below)
      return rhs.pipe == Pipe::kVertical || rhs.pipe == Pipe::kUpRight || rhs.pipe == Pipe::kUpLeft;
    if (is_rhs_left)
      return rhs.pipe == Pipe::kHorizontal || rhs.pipe == Pipe::kDownRight || rhs.pipe == Pipe::kUpRight;
    if (is_rhs_right)
      return rhs.pipe == Pipe::kHorizontal || rhs.pipe == Pipe::kDownLeft || rhs.pipe == Pipe::kUpLeft;
  }
  if (rhs.pipe == Pipe::kStart) {
    if (is_rhs_above)
      return lhs.pipe == Pipe::kVertical || lhs.pipe == Pipe::kUpRight || lhs.pipe == Pipe::kUpLeft;
    if (is_rhs_below)
      return lhs.pipe == Pipe::kVertical || lhs.pipe == Pipe::kDownRight || lhs.pipe == Pipe::kDownLeft;
    if (is_rhs_left)
      return lhs.pipe == Pipe::kHorizontal || lhs.pipe == Pipe::kUpLeft || lhs.pipe == Pipe::kDownLeft;
    if (is_rhs_right)
      return lhs.pipe == Pipe::kHorizontal || lhs.pipe == Pipe::kUpRight || lhs.pipe == Pipe::kDownRight;
  }
  if (is_rhs_above)
    return (lhs.pipe == Pipe::kVertical || lhs.pipe == Pipe::kUpRight || lhs.pipe == Pipe::kUpLeft) &&
           (rhs.pipe == Pipe::kVertical || rhs.pipe == Pipe::kDownRight || rhs.pipe == Pipe::kDownLeft);
  if (is_rhs_below)
    return (lhs.pipe == Pipe::kVertical || lhs.pipe == Pipe::kDownRight || lhs.pipe == Pipe::kDownLeft) &&
           (rhs.pipe == Pipe::kVertical || rhs.pipe == Pipe::kUpRight || rhs.pipe == Pipe::kUpLeft);
  if (is_rhs_left)
    return (lhs.pipe == Pipe::kHorizontal || lhs.pipe == Pipe::kUpLeft || lhs.pipe == Pipe::kDownLeft) &&
           (rhs.pipe == Pipe::kHorizontal || rhs.pipe == Pipe::kUpRight || rhs.pipe == Pipe::kDownRight);
  if (is_rhs_right)
    return (lhs.pipe == Pipe::kHorizontal || lhs.pipe == Pipe::kUpRight || lhs.pipe == Pipe::kDownRight) &&
           (rhs.pipe == Pipe::kHorizontal || rhs.pipe == Pipe::kUpLeft || rhs.pipe == Pipe::kDownLeft);
  return false;
}

std::int32_t Graph::BFSMaxDistance() {
  std::unordered_set<Vertex*> visited{};
  std::queue<Vertex*> q{};
  q.push(&vertices[start.first][start.second]);
  visited.insert(&vertices[start.first][start.second]);
  vertices[start.first][start.second].distance = 0;
  while (!q.empty()) {
    const auto vertex = q.front();
    q.pop();
    if (vertex->position.first > 0) {  // Up
      auto& neighbour = vertices[vertex->position.first - 1][vertex->position.second];
      if (Vertex::AreConnected(*vertex, neighbour) && !visited.contains(&neighbour)) {
        neighbour.distance = vertex->distance + 1;
        visited.insert(&neighbour);
        q.push(&neighbour);
      }
    }
    if (vertex->position.first < vertices.size() - 1) {  // Down
      auto& neighbour = vertices[vertex->position.first + 1][vertex->position.second];
      if (Vertex::AreConnected(*vertex, neighbour) && !visited.contains(&neighbour)) {
        neighbour.distance = vertex->distance + 1;
        visited.insert(&neighbour);
        q.push(&neighbour);
      }
    }
    if (vertex->position.second > 0) {  // Left
      auto& neighbour = vertices[vertex->position.first][vertex->position.second - 1];
      if (Vertex::AreConnected(*vertex, neighbour) && !visited.contains(&neighbour)) {
        neighbour.distance = vertex->distance + 1;
        visited.insert(&neighbour);
        q.push(&neighbour);
      }
    }
    if (vertex->position.second < vertices[vertex->position.first].size() - 1) {  // Right
      auto& neighbour = vertices[vertex->position.first][vertex->position.second + 1];
      if (Vertex::AreConnected(*vertex, neighbour) && !visited.contains(&neighbour)) {
        neighbour.distance = vertex->distance + 1;
        visited.insert(&neighbour);
        q.push(&neighbour);
      }
    }
  }
  std::int32_t max_distance{0};
  std::ranges::for_each(vertices, [&max_distance](const auto& row) {
    std::ranges::for_each(
        row, [&max_distance](const auto& vertex) { max_distance = std::max(max_distance, vertex.distance); });
  });
  return max_distance;
}

Graph Graph::Parse(std::istream& is) {
  Graph graph{};
  std::string line{};
  while (std::getline(is, line) && !line.empty()) {
    std::vector<Vertex> row{};
    row.reserve(line.size());
    std::size_t column_index{0};
    std::ranges::for_each(line, [&graph, &row, &column_index](const char c) {
      Vertex vertex{};
      vertex.position = {graph.vertices.size(), column_index};
      switch (c) {
        case '|':
          vertex.pipe = Pipe::kVertical;
          break;
        case '-':
          vertex.pipe = Pipe::kHorizontal;
          break;
        case 'L':
          vertex.pipe = Pipe::kUpRight;
          break;
        case 'J':
          vertex.pipe = Pipe::kUpLeft;
          break;
        case 'F':
          vertex.pipe = Pipe::kDownRight;
          break;
        case '7':
          vertex.pipe = Pipe::kDownLeft;
          break;
        case 'S':
          vertex.pipe = Pipe::kStart;
          graph.start = vertex.position;
          break;
        default:
          vertex.pipe = std::nullopt;
          break;
      }
      ++column_index;
      row.emplace_back(std::move(vertex));
    });
    graph.vertices.emplace_back(std::move(row));
  }
  return graph;
}

int main() {
  auto graph = Graph::Parse(std::cin);
  std::cout << graph.BFSMaxDistance() << std::endl;
}
