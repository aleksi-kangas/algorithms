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
    bool is_inside_loop{true};

    [[nodiscard]] bool IsOriginalTile() const;
    [[nodiscard]] static bool AreConnected(const Vertex& lhs, const Vertex& rhs);
  };

  std::vector<std::vector<Vertex>> vertices{};

  [[nodiscard]] std::pair<std::size_t, std::size_t> Start() const;
  [[nodiscard]] bool IsAtEdge(const Vertex& vertex) const;

  void EraseJunkPipes();
  void FloodFromEdges();

  [[nodiscard]] static Graph Parse(std::istream& is);
  [[nodiscard]] static Graph Expand(Graph&& graph);

 private:
  [[nodiscard]] static Graph ExpandHorizontally(Graph&& graph);
  [[nodiscard]] static Graph ExpandVertically(Graph&& graph);
};

bool Graph::Vertex::IsOriginalTile() const {
  if (pipe.has_value())
    return false;
  return position.first % 2 == 0 && position.second % 2 == 0 && is_inside_loop;
}

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

std::pair<std::size_t, std::size_t> Graph::Start() const {
  for (std::size_t r{0}; r < vertices.size(); ++r) {
    for (std::size_t c{0}; c < vertices[r].size(); ++c) {
      if (vertices[r][c].pipe == Pipe::kStart)
        return {r, c};
    }
  }
  throw std::runtime_error{"Start not found"};
}

bool Graph::IsAtEdge(const Vertex& vertex) const {
  return vertex.position.first == 0 || vertex.position.first == vertices.size() - 1 || vertex.position.second == 0 ||
         vertex.position.second == vertices[vertex.position.first].size() - 1;
}

void Graph::EraseJunkPipes() {
  std::unordered_set<Vertex*> visited{};
  std::queue<Vertex*> q{};
  const auto start = Start();
  q.push(&vertices[start.first][start.second]);
  visited.insert(&vertices[start.first][start.second]);
  while (!q.empty()) {
    const auto vertex = q.front();
    q.pop();
    if (vertex->position.first > 0) {  // Up
      auto& neighbour = vertices[vertex->position.first - 1][vertex->position.second];
      if (Vertex::AreConnected(*vertex, neighbour) && !visited.contains(&neighbour)) {
        visited.insert(&neighbour);
        q.push(&neighbour);
      }
    }
    if (vertex->position.first < vertices.size() - 1) {  // Down
      auto& neighbour = vertices[vertex->position.first + 1][vertex->position.second];
      if (Vertex::AreConnected(*vertex, neighbour) && !visited.contains(&neighbour)) {
        visited.insert(&neighbour);
        q.push(&neighbour);
      }
    }
    if (vertex->position.second > 0) {  // Left
      auto& neighbour = vertices[vertex->position.first][vertex->position.second - 1];
      if (Vertex::AreConnected(*vertex, neighbour) && !visited.contains(&neighbour)) {
        visited.insert(&neighbour);
        q.push(&neighbour);
      }
    }
    if (vertex->position.second < vertices[vertex->position.first].size() - 1) {  // Right
      auto& neighbour = vertices[vertex->position.first][vertex->position.second + 1];
      if (Vertex::AreConnected(*vertex, neighbour) && !visited.contains(&neighbour)) {
        visited.insert(&neighbour);
        q.push(&neighbour);
      }
    }
  }
  for (std::size_t r{0}; r < vertices.size(); ++r) {
    for (std::size_t c{0}; c < vertices[r].size(); ++c) {
      if (vertices[r][c].pipe.has_value() && !visited.contains(&vertices[r][c])) {
        vertices[r][c].pipe.reset();
      }
    }
  }
}

void Graph::FloodFromEdges() {
  std::unordered_set<Vertex*> visited{};
  std::queue<Vertex*> q{};
  for (std::size_t r{0}; r < vertices.size(); ++r) {
    for (std::size_t c{0}; c < vertices[r].size(); ++c) {
      if (IsAtEdge(vertices[r][c]) && !vertices[r][c].pipe.has_value()) {
        q.push(&vertices[r][c]);
        visited.insert(&vertices[r][c]);
        vertices[r][c].is_inside_loop = false;
      }
    }
  }
  while (!q.empty()) {
    const auto vertex = q.front();
    q.pop();

    if (vertex->position.first > 0) {  // Up
      auto& neighbour = vertices[vertex->position.first - 1][vertex->position.second];
      if (!neighbour.pipe.has_value() && !visited.contains(&neighbour)) {
        neighbour.is_inside_loop = false;
        q.push(&neighbour);
        visited.insert(&neighbour);
      }
    }
    if (vertex->position.first < vertices.size() - 1) {  // Down
      auto& neighbour = vertices[vertex->position.first + 1][vertex->position.second];
      if (!neighbour.pipe.has_value() && !visited.contains(&neighbour)) {
        neighbour.is_inside_loop = false;
        q.push(&neighbour);
        visited.insert(&neighbour);
      }
    }
    if (vertex->position.second > 0) {  // Left
      auto& neighbour = vertices[vertex->position.first][vertex->position.second - 1];
      if (!neighbour.pipe.has_value() && !visited.contains(&neighbour)) {
        neighbour.is_inside_loop = false;
        q.push(&neighbour);
        visited.insert(&neighbour);
      }
    }
    if (vertex->position.second < vertices[vertex->position.first].size() - 1) {  // Right
      auto& neighbour = vertices[vertex->position.first][vertex->position.second + 1];
      if (!neighbour.pipe.has_value() && !visited.contains(&neighbour)) {
        neighbour.is_inside_loop = false;
        q.push(&neighbour);
        visited.insert(&neighbour);
      }
    }
  }
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

Graph Graph::Expand(Graph&& graph) {
  return ExpandVertically(ExpandHorizontally(std::move(graph)));
}

Graph Graph::ExpandHorizontally(Graph&& graph) {
  Graph expanded_graph{};
  for (std::size_t r{0}; r < graph.vertices.size(); ++r) {
    std::vector<Vertex> row{};
    row.reserve(graph.vertices[r].size() * 2 - 1);
    for (std::size_t c{0}; c < graph.vertices[r].size(); ++c) {
      Vertex original_vertex = graph.vertices[r][c];
      original_vertex.position = {r, c * 2};
      row.emplace_back(std::move(original_vertex));
      if (c < graph.vertices[r].size() - 1) {
        Vertex new_vertex{};
        new_vertex.position = {r, c * 2 + 1};
        new_vertex.pipe = Vertex::AreConnected(graph.vertices[r][c], graph.vertices[r][c + 1])
                              ? std::make_optional(Pipe::kHorizontal)
                              : std::nullopt;
        row.emplace_back(std::move(new_vertex));
      }
    }
    expanded_graph.vertices.emplace_back(std::move(row));
  }
  return expanded_graph;
}

Graph Graph::ExpandVertically(Graph&& graph) {
  Graph expanded_graph{};
  expanded_graph.vertices.reserve(graph.vertices.size() * 2 - 1);
  for (std::size_t r{0}; r < graph.vertices.size(); ++r) {
    std::vector<Vertex> original_row = graph.vertices[r];
    for (std::size_t c{0}; c < graph.vertices[r].size(); ++c) {
      original_row[c].position = {r * 2, c};
    }
    expanded_graph.vertices.emplace_back(std::move(original_row));
    if (r < graph.vertices.size() - 1) {
      std::vector<Vertex> new_row{};
      new_row.reserve(graph.vertices[r].size());
      for (std::size_t j{0}; j < graph.vertices[r].size(); ++j) {
        Vertex new_vertex{};
        new_vertex.position = {r * 2 + 1, j};
        new_vertex.pipe = Vertex::AreConnected(graph.vertices[r][j], graph.vertices[r + 1][j])
                              ? std::make_optional(Pipe::kVertical)
                              : std::nullopt;
        new_row.emplace_back(std::move(new_vertex));
      }
      expanded_graph.vertices.emplace_back(std::move(new_row));
    }
  }
  return expanded_graph;
}

int main() {
  auto graph = Graph::Expand(Graph::Parse(std::cin));
  graph.EraseJunkPipes();
  graph.FloodFromEdges();
  std::int32_t tile_count{0};
  std::ranges::for_each(graph.vertices, [&](const auto& row) {
    std::ranges::for_each(row, [&](const auto& vertex) { tile_count += vertex.IsOriginalTile(); });
  });
  std::cout << tile_count << std::endl;
}
