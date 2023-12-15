#include <cstdint>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// On some platforms, need to redirect input from a file:
// ./<executable_name> < <input_file_name>, e.g. ./15_1_lens_library < in.txt

std::size_t Hash(std::string_view s) {
  std::size_t hash{0};
  for (const auto c : s) {
    hash += static_cast<std::int32_t>(c);
    hash *= 17;
    hash %= 256;
  }
  return hash;
}

std::vector<std::string> ReadInitializationSequence(std::istream& is) {
  std::vector<std::string> initialization_sequence{};
  std::string line{};
  std::getline(is, line);
  std::istringstream iss{line};
  std::string segment{};
  while (std::getline(iss, segment, ',')) {
    initialization_sequence.push_back(std::move(segment));
  }
  return initialization_sequence;
}

int main() {
  const auto initialization_sequence = ReadInitializationSequence(std::cin);
  std::size_t hash_sum{0};
  for (const auto& segment : initialization_sequence) {
    hash_sum += Hash(segment);
  }
  std::cout << hash_sum << std::endl;
}
