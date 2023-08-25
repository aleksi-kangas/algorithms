#include <charconv>
#include <iostream>
#include <string>

enum class Direction { Forward, Down, Up };

struct Command {
  Direction direction{Direction::Forward};
  std::int32_t value{0};
};

Command ParseCommand(std::string_view s) {
  const auto split = s.find(' ');
  if (split == std::string_view::npos)
    throw std::invalid_argument{"Invalid command"};
  Command command{};
  const std::string_view direction = s.substr(0, split);
  if (direction == "down")
    command.direction = Direction::Down;
  else if (direction == "up")
    command.direction = Direction::Up;
  else if (direction == "forward")
    command.direction = Direction::Forward;
  else
    throw std::invalid_argument{"Invalid direction"};
  const auto [_, ec] = std::from_chars(s.data() + split + 1, s.data() + s.size(),
                                       command.value);
  if (ec != std::errc{})
    throw std::invalid_argument{"Invalid value"};
  return command;
}

int main() {
  std::int32_t x{0};
  std::int32_t depth{0};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    const auto command = ParseCommand(line);
    switch (command.direction) {
      case Direction::Forward:
        x += command.value;
        break;
      case Direction::Down:
        depth += command.value;
        break;
      case Direction::Up:
        depth -= command.value;
        break;
    }
  }
  const std::int32_t multiplication = x * depth;
  std::cout << multiplication << std::endl;
}
