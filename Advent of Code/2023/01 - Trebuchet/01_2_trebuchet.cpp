#include <cstdint>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <string>
#include <utility>

std::pair<std::size_t, std::int32_t> FindFirstDigit(std::string_view line) {
  const auto digit_position = line.find_first_of("0123456789");
  return {digit_position, digit_position != std::string::npos ? line[digit_position] - '0' : 0};
}

std::pair<std::size_t, std::int32_t> FindLastDigit(std::string_view line) {
  const auto digit_position = line.find_last_of("0123456789");
  return {digit_position, digit_position != std::string::npos ? line[digit_position] - '0' : 0};
}

std::pair<std::size_t, std::int32_t> FindFirstSpelledOutDigit(std::string_view line) {
  for (std::size_t position = 0; position < line.size(); ++position) {
    const std::string_view candidate = line.substr(position);
    if (candidate.starts_with("one"))
      return {position, 1};
    if (candidate.starts_with("two"))
      return {position, 2};
    if (candidate.starts_with("three"))
      return {position, 3};
    if (candidate.starts_with("four"))
      return {position, 4};
    if (candidate.starts_with("five"))
      return {position, 5};
    if (candidate.starts_with("six"))
      return {position, 6};
    if (candidate.starts_with("seven"))
      return {position, 7};
    if (candidate.starts_with("eight"))
      return {position, 8};
    if (candidate.starts_with("nine"))
      return {position, 9};
  }
  return {std::string::npos, 0};
}

std::pair<std::size_t, std::int32_t> FindLastSpelledOutDigit(std::string_view line) {
  constexpr std::size_t window_start{0};
  std::size_t window_end{line.size() - 1};
  while (window_start != window_end) {
    const auto candidate = line.substr(window_start, window_end - window_start + 1);
    if (candidate.ends_with("one"))
      return {window_end - 2, 1};
    if (candidate.ends_with("two"))
      return {window_end - 2, 2};
    if (candidate.ends_with("three"))
      return {window_end - 4, 3};
    if (candidate.ends_with("four"))
      return {window_end - 3, 4};
    if (candidate.ends_with("five"))
      return {window_end - 3, 5};
    if (candidate.ends_with("six"))
      return {window_end - 2, 6};
    if (candidate.ends_with("seven"))
      return {window_end - 4, 7};
    if (candidate.ends_with("eight"))
      return {window_end - 4, 8};
    if (candidate.ends_with("nine"))
      return {window_end - 3, 9};
    --window_end;
  }
  return {std::string::npos, 0};
}

std::int32_t FindDigit(std::string_view line, bool first) {
  const auto [digit_position, digit] = first ? FindFirstDigit(line) : FindLastDigit(line);
  const auto [spelled_out_digit_position, spelled_out_digit] =
      first ? FindFirstSpelledOutDigit(line) : FindLastSpelledOutDigit(line);
  const bool found_digit = digit_position != std::string::npos;
  const bool found_spelled_out_digit = spelled_out_digit_position != std::string::npos;
  if (!found_digit && !found_spelled_out_digit)
    throw std::invalid_argument{"No digit found in line"};
  if (!found_digit)
    return spelled_out_digit;
  if (!found_spelled_out_digit)
    return digit;
  if (first) {
    return digit_position < spelled_out_digit_position ? digit : spelled_out_digit;
  } else {
    return digit_position > spelled_out_digit_position ? digit : spelled_out_digit;
  }
}

std::int32_t CalibrationValue(std::string_view line) {
  const std::int32_t first_digit = FindDigit(line, true);
  const std::int32_t second_digit = FindDigit(line, false);
  return first_digit * 10 + second_digit;
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
