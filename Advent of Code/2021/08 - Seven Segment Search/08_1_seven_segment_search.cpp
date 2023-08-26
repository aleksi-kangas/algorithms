#include <iostream>
#include <sstream>
#include <string>

std::int32_t CountOutputDigitsWithUnique(std::string_view s) {
  std::int32_t count{0};
  const auto delimiter = s.find('|');
  std::stringstream ss{std::string{s.substr(delimiter + 1)}};
  for (std::string value; ss >> value;) {
    switch (value.size()) {
      case 2:  // 1
      case 4:  // 4
      case 3:  // 7
      case 7:  // 8
        ++count;
        break;
    }
    if (ss.peek() == ' ') {
      ss.ignore();
    }
  }
  return count;
}

int main() {
  std::int32_t answer{0};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    answer += CountOutputDigitsWithUnique(line);
  }
  std::cout << answer << std::endl;
}
