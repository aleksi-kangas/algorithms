#include <cstdint>
#include <iostream>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

struct Game {
  struct Draw {
    std::int32_t red{0};
    std::int32_t green{0};
    std::int32_t blue{0};
  };

  std::int32_t id{0};
  std::vector<Draw> draws{};

  static constexpr std::int32_t kMaxRed{12};
  static constexpr std::int32_t kMaxGreen{13};
  static constexpr std::int32_t kMaxBlue{14};

  [[nodiscard]] bool Possible() const {
    return std::ranges::all_of(draws, [](const auto& draw) {
      return draw.red <= kMaxRed && draw.green <= kMaxGreen && draw.blue <= kMaxBlue;
    });
  }

  [[nodiscard]] static Game Parse(std::string_view line) {
    const auto colon_position = line.find(':');
    if (colon_position == std::string::npos) {
      throw std::invalid_argument{"No colon found in line"};
    }
    const auto id = std::stoi(std::string{line.substr(5, colon_position - 5)});
    const auto draws = ParseDraws(line.substr(colon_position + 2));
    return {.id = id, .draws = draws};
  }

 private:
  [[nodiscard]] static std::vector<Draw> ParseDraws(std::string_view s) {
    std::vector<Draw> draws{};
    for (const auto& draw : Split(s, ';')) {
      draws.push_back(ParseDraw(draw));
    }
    return draws;
  }

  [[nodiscard]] static Draw ParseDraw(std::string_view s) {
    Draw draw{};
    for (const auto& sub_draw : Split(s, ',')) {
      if (sub_draw.ends_with("red")) {
        draw.red = std::stoi(std::string{sub_draw.substr(0, sub_draw.size() - 3)});
      }
      if (sub_draw.ends_with("green")) {
        draw.green = std::stoi(std::string{sub_draw.substr(0, sub_draw.size() - 5)});
      }
      if (sub_draw.ends_with("blue")) {
        draw.blue = std::stoi(std::string{sub_draw.substr(0, sub_draw.size() - 4)});
      }
    }
    return draw;
  }

  [[nodiscard]] static std::vector<std::string> Split(std::string_view s, char delimiter) {
    std::stringstream ss{std::string{s}};
    std::string segment{};
    std::vector<std::string> segments{};
    while (std::getline(ss, segment, delimiter)) {
      segments.push_back(std::move(segment));
    }
    return segments;
  }
};

int main() {
  std::int32_t sum{0};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    const auto game = Game::Parse(line);
    if (game.Possible()) {
      sum += game.id;
    }
  }
  std::cout << sum << std::endl;
}
