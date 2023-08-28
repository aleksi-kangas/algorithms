#include <algorithm>
#include <cstdint>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

bool IsCorrupt(char close, char open) {
  switch (close) {
    case ')':
      return open != '(';
    case ']':
      return open != '[';
    case '}':
      return open != '{';
    case '>':
      return open != '<';
    default:
      throw std::runtime_error{"Invalid character"};
  }
}

std::int64_t CompletionPenalty(std::stack<char>& s) {
  std::int64_t penalty{0};
  while (!s.empty()) {
    penalty *= 5;
    const char c = s.top();
    s.pop();
    switch (c) {
      case '(':
        penalty += 1;
        break;
      case '[':
        penalty += 2;
        break;
      case '{':
        penalty += 3;
        break;
      case '<':
        penalty += 4;
        break;
      default:
        throw std::runtime_error{"Invalid character"};
    }
  }
  return penalty;
}

int main() {
  std::vector<std::int64_t> penalties{};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    std::stack<char> s{};
    bool is_corrupt{false};
    for (const char c : line) {
      if (is_corrupt)
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
          if (IsCorrupt(c, open)) {
            is_corrupt = true;
            break;
          }
        } break;
        default:
          throw std::runtime_error{"Invalid character"};
      }
    }
    if (!is_corrupt) {
      penalties.push_back(CompletionPenalty(s));
    }
  }
  std::sort(penalties.begin(), penalties.end());
  std::cout << penalties[penalties.size() / 2] << '\n';
}
