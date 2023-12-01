#include <algorithm>
#include <cstdint>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <string>

std::int32_t CalibrationValue(std::string_view line) {
  const auto first_digit = std::ranges::find_first_of(line, "0123456789");
  if (first_digit == line.end()) {
    throw std::invalid_argument{"No digit found in line"};
  }
  const auto second_digit = std::ranges::find_first_of(line | std::ranges::views::reverse, "0123456789");
  if (second_digit == line.rend()) {
    throw std::invalid_argument{"No digit found in line"};
  }
  return std::stoi(std::string{*first_digit, *second_digit});
}

int main() {
  std::int32_t sum{0};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    const auto calibration_value = CalibrationValue(line);
    sum += calibration_value;
  }
  std::cout << sum << std::endl;
}
