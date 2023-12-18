#include <cstdint>
#include <iostream>
#include <istream>
#include <limits>
#include <optional>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

using position_t = std::pair<std::size_t, std::size_t>;

template <>
struct std::hash<position_t> {
  std::size_t operator()(const position_t& p) const noexcept {
    const std::size_t h1 = std::hash<std::size_t>{}(p.first);
    const std::size_t h2 = std::hash<std::size_t>{}(p.second);
    return h1 ^ (h2 << 1);
  }
};

enum class Direction { kUp, kDown, kLeft, kRight };

struct Vertex {
  position_t position{};
  std::size_t value{};
};

struct State {
  Vertex* vertex;
  Direction direction{};
  std::size_t distance{0};
  std::size_t loss{std::numeric_limits<std::size_t>::max()};

  bool operator==(const State& rhs) const noexcept {
    return vertex == rhs.vertex && direction == rhs.direction && distance == rhs.distance;
  }
};

template <>
struct std::hash<State> {
  std::size_t operator()(const State& s) const noexcept {
    const std::size_t h1 = std::hash<Vertex*>{}(s.vertex);
    const std::size_t h2 = std::hash<std::int32_t>{}(static_cast<std::int32_t>(s.direction));
    const std::size_t h3 = std::hash<std::size_t>{}(s.distance);
    return h1 ^ (h2 << 1) ^ (h3 << 2);
  }
};

std::optional<State> MoveUp(const State& state, std::vector<std::vector<Vertex>>& map) {
  if (state.vertex->position.first == 0 || state.direction == Direction::kDown)
    return std::nullopt;
  if (state.direction == Direction::kUp && state.distance == 3)
    return std::nullopt;
  Vertex* above = &map[state.vertex->position.first - 1][state.vertex->position.second];
  return State{.vertex = above,
               .direction = Direction::kUp,
               .distance = state.direction == Direction::kUp ? state.distance + 1 : 1,
               .loss = state.loss + above->value};
}

std::optional<State> MoveDown(const State& state, std::vector<std::vector<Vertex>>& map) {
  if (state.vertex->position.first == map.size() - 1 || state.direction == Direction::kUp)
    return std::nullopt;
  if (state.direction == Direction::kDown && state.distance == 3)
    return std::nullopt;
  Vertex* below = &map[state.vertex->position.first + 1][state.vertex->position.second];
  return State{.vertex = below,
               .direction = Direction::kDown,
               .distance = state.direction == Direction::kDown ? state.distance + 1 : 1,
               .loss = state.loss + below->value};
}

std::optional<State> MoveLeft(const State& state, std::vector<std::vector<Vertex>>& map) {
  if (state.vertex->position.second == 0 || state.direction == Direction::kRight)
    return std::nullopt;
  if (state.direction == Direction::kLeft && state.distance == 3)
    return std::nullopt;
  Vertex* left = &map[state.vertex->position.first][state.vertex->position.second - 1];
  return State{.vertex = left,
               .direction = Direction::kLeft,
               .distance = state.direction == Direction::kLeft ? state.distance + 1 : 1,
               .loss = state.loss + left->value};
}

std::optional<State> MoveRight(const State& state, std::vector<std::vector<Vertex>>& map) {
  if (state.vertex->position.second == map[0].size() - 1 || state.direction == Direction::kLeft)
    return std::nullopt;
  if (state.direction == Direction::kRight && state.distance == 3)
    return std::nullopt;
  Vertex* right = &map[state.vertex->position.first][state.vertex->position.second + 1];
  return State{.vertex = right,
               .direction = Direction::kRight,
               .distance = state.direction == Direction::kRight ? state.distance + 1 : 1,
               .loss = state.loss + right->value};
}

std::vector<std::vector<Vertex>> ReadMap(std::istream& is) {
  std::vector<std::vector<Vertex>> map{};
  std::string line{};
  while (std::getline(is, line) && !line.empty()) {
    std::vector<Vertex> row{};
    row.reserve(line.size());
    for (const auto& c : line) {
      row.emplace_back(Vertex{.position = {map.size(), row.size()}, .value = static_cast<std::size_t>(c - '0')});
    }
    map.emplace_back(std::move(row));
  }
  return map;
}

std::size_t MinimumHeatLoss(std::vector<std::vector<Vertex>>& m) {
  std::unordered_set<State> visited{};
  const auto PQComparator = [](const State& lhs, const State& rhs) -> std::int32_t {
    return lhs.loss > rhs.loss;  // Minimum PQ
  };
  std::priority_queue<State, std::vector<State>, decltype(PQComparator)> pq{PQComparator};
  pq.emplace(State{.vertex = &m[0][0], .direction = Direction::kRight, .distance = 0, .loss = 0});
  State s{};
  while (!pq.empty()) {
    s = pq.top();
    pq.pop();
    if (s.vertex == &m.back().back())
      break;
    const auto state_it = visited.find(s);
    if (state_it != visited.end() && state_it->loss <= s.loss)
      continue;
    visited.insert(s);
    if (auto next = MoveUp(s, m); next.has_value()) {
      pq.emplace(next.value());
    }
    if (auto next = MoveDown(s, m); next.has_value()) {
      pq.emplace(next.value());
    }
    if (auto next = MoveLeft(s, m); next.has_value()) {
      pq.emplace(next.value());
    }
    if (auto next = MoveRight(s, m); next.has_value()) {
      pq.emplace(next.value());
    }
  }
  return s.loss;
}

int main() {
  auto map = ReadMap(std::cin);
  std::cout << MinimumHeatLoss(map) << std::endl;
}
