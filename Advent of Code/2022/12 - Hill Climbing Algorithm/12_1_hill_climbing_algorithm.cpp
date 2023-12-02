#include <cstdint>
#include <iostream>
#include <queue>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

struct Graph {
  struct Vertex {
    std::pair<std::size_t, std::size_t> position{};
    char id{'a'};
    std::int32_t elevation{0};
    std::size_t steps{0};
    std::vector<Vertex*> neighbors{};
  };

  std::vector<std::vector<Vertex>> grid{};
  std::pair<std::size_t, std::size_t> start_position{};
  std::pair<std::size_t, std::size_t> end_position{};

  [[nodiscard]] static Graph Parse(std::istream& is) {
    Graph graph{};
    std::string line{};
    std::size_t row{0};
    while (std::getline(is, line) && !line.empty()) {
      graph.grid.push_back(ParseLine(line, row++));
    }
    graph.ConnectNeighbors();
    graph.FindStartAndEndPositions();
    return graph;
  }

  [[nodiscard]] std::size_t MinimumStepsBFS() {
    std::ranges::for_each(grid, [](std::vector<Vertex>& row_vertices) {
      std::ranges::for_each(row_vertices, [](Vertex& vertex) { vertex.steps = 0; });
    });
    std::vector<std::vector<bool>> visited(grid.size(), std::vector<bool>(grid[0].size(), false));
    std::queue<Vertex*> q{};
    q.push(&grid[start_position.first][start_position.second]);
    visited[start_position.first][start_position.second] = true;
    while (!q.empty()) {
      const auto vertex = q.front();
      q.pop();
      for (const auto& neighbor : vertex->neighbors) {
        if (!visited[neighbor->position.first][neighbor->position.second]) {
          neighbor->steps = vertex->steps + 1;
          visited[neighbor->position.first][neighbor->position.second] = true;
          q.push(neighbor);
        }
      }
    }
    return grid[end_position.first][end_position.second].steps;
  }

 private:
  [[nodiscard]] static std::vector<Vertex> ParseLine(std::string_view line, std::size_t row) {
    std::vector<Vertex> vertices{};
    std::size_t column{0};
    for (const char c : line) {
      std::int32_t elevation{0};
      switch (c) {
        case 'S':
          elevation = 'a' - 'a';
          break;
        case 'E':
          elevation = 'z' - 'a';
          break;
        default:
          elevation = c - 'a';
          break;
      }
      vertices.push_back(Vertex{.position = {row, column++}, .id = c, .elevation = elevation});
    }
    return vertices;
  }

  void ConnectNeighbors() {
    std::ranges::for_each(grid, [](std::vector<Vertex>& row_vertices) {
      std::ranges::for_each(row_vertices, [](Vertex& vertex) { vertex.neighbors.clear(); });
    });
    for (std::size_t r = 0; r < grid.size(); ++r) {
      for (std::size_t c = 0; c < grid[0].size(); ++c) {
        if (const bool up = r > 0 && grid[r][c].elevation - grid[r - 1][c].elevation >= -1) {
          grid[r][c].neighbors.push_back(&grid[r - 1][c]);
        }
        if (const bool down = r < grid.size() - 1 && grid[r][c].elevation - grid[r + 1][c].elevation >= -1) {
          grid[r][c].neighbors.push_back(&grid[r + 1][c]);
        }
        if (const bool left = c > 0 && grid[r][c].elevation - grid[r][c - 1].elevation >= -1) {
          grid[r][c].neighbors.push_back(&grid[r][c - 1]);
        }
        if (const bool right = c < grid[0].size() - 1 && grid[r][c].elevation - grid[r][c + 1].elevation >= -1) {
          grid[r][c].neighbors.push_back(&grid[r][c + 1]);
        }
      }
    }
  }

  void FindStartAndEndPositions() {
    for (std::size_t r = 0; r < grid.size(); ++r) {
      for (std::size_t c = 0; c < grid[0].size(); ++c) {
        if (grid[r][c].id == 'S') {
          start_position = {r, c};
        } else if (grid[r][c].id == 'E') {
          end_position = {r, c};
        }
      }
    }
  }
};

int main() {
  auto graph = Graph::Parse(std::cin);
  std::cout << graph.MinimumStepsBFS() << std::endl;
}
