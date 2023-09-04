#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>

std::pair<std::int32_t, std::int32_t> ReadInitialPositions() {
  std::string p1{};
  std::getline(std::cin, p1);
  const auto p1_colon_index = p1.find(':');
  std::string p2{};
  std::getline(std::cin, p2);
  const auto p2_colon_index = p2.find(':');
  return {std::stoi(p1.substr(p1_colon_index + 2)), std::stoi(p2.substr(p2_colon_index + 2))};
}

// We could write a recursive brute-force approach and optimize it using DP.
// Instead, I'll opt for a more mathematical approach.
// Both players can have positions in the range [1, 10] and scores in the range [0, 20].
// Thus, there are 10 * 10 * 21 * 21 * 2 = 88200 possible game states.

struct GameState {
  std::pair<std::int32_t, std::int32_t> positions{};
  std::pair<std::int32_t, std::int32_t> scores{};
  bool p1{true};

  [[nodiscard]] bool IsOver() const { return scores.first >= 21 || scores.second >= 21; }

  auto operator<=>(const GameState&) const = default;
};

template <class T>
inline void HashCombine(std::size_t& seed, const T& v) {
  seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <>
struct std::hash<GameState> {
  std::size_t operator()(const GameState& gs) const noexcept {
    std::size_t hash{0};
    HashCombine(hash, gs.positions.first);
    HashCombine(hash, gs.positions.second);
    HashCombine(hash, gs.scores.first);
    HashCombine(hash, gs.scores.second);
    HashCombine(hash, gs.p1);
    return hash;
  }
};

GameState CloneGameState(const GameState& state, std::int32_t roll, bool p1) {
  GameState clone = state;
  if (p1) {
    clone.positions.first += roll;
    clone.positions.first %= 10;
    if (clone.positions.first == 0) {
      clone.positions.first = 10;
    }
    clone.scores.first += clone.positions.first;
    clone.p1 = false;
  } else {
    clone.positions.second += roll;
    clone.positions.second %= 10;
    if (clone.positions.second == 0) {
      clone.positions.second = 10;
    }
    clone.scores.second += clone.positions.second;
    clone.p1 = true;
  }
  return clone;
}

int main() {
  const auto initial_positions = ReadInitialPositions();
  std::unordered_map<GameState, std::int64_t> resolved_game_states{};
  std::unordered_map<GameState, std::int64_t> unresolved_game_states{};
  unresolved_game_states[GameState{initial_positions, {0, 0}, true}] = 1;
  while (!unresolved_game_states.empty()) {
    std::unordered_map<GameState, std::int64_t> new_unresolved_game_states{};
    for (const auto& [state, count] : unresolved_game_states) {
      std::int32_t sum{0};
      for (std::int32_t r1 = 1; r1 <= 3; ++r1) {
        sum += r1;
        for (std::int32_t r2 = 1; r2 <= 3; ++r2) {
          sum += r2;
          for (std::int32_t r3 = 1; r3 <= 3; ++r3) {
            sum += r3;

            auto clone = CloneGameState(state, sum, state.p1);
            if (clone.IsOver()) {
              resolved_game_states[clone] += count;
            } else {
              new_unresolved_game_states[clone] += count;
            }

            sum -= r3;
          }
          sum -= r2;
        }
        sum -= r1;
      }
    }
    unresolved_game_states = std::move(new_unresolved_game_states);
  }
  std::int64_t p1_wins{0};
  std::int64_t p2_wins{0};
  for (const auto& [state, count] : resolved_game_states) {
    if (state.scores.first > state.scores.second) {
      p1_wins += count;
    } else {
      p2_wins += count;
    }
  }
  std::cout << std::max(p1_wins, p2_wins) << '\n';
}
