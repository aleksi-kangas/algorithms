#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>

std::int32_t Penalty(char close, char open) {
  switch (close) {
    case ')':
      return open == '(' ? 0 : 3;
    case ']':
      return open == '[' ? 0 : 57;
    case '}':
      return open == '{' ? 0 : 1197;
    case '>':
      return open == '<' ? 0 : 25137;
    default:
      throw std::runtime_error{"Invalid character"};
  }
}

int main() {
  std::int64_t total_penalty{};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    std::stack<char> s{};
    bool valid{true};
    for (const char c : line) {
      if (!valid)
        break;
      switch (c) {
        case '(':
        case '[':
        case '{':
        case '<': {
          s.push(c);
        } break;
        case ')':
        case ']':
        case '}':
        case '>': {
          const char open = s.top();
          s.pop();
          const auto penalty = Penalty(c, open);
          total_penalty += penalty;
          if (penalty > 0) {
            valid = false;
          }
        } break;
        default:
          throw std::runtime_error{"Invalid character"};
      }
    }
  }
  std::cout << total_penalty << '\n';
}
