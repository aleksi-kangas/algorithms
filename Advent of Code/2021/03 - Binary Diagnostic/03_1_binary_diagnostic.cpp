#include <array>
#include <bitset>
#include <iostream>
#include <string>

constexpr std::size_t kBits{12};

int main() {
  std::int32_t count{0};
  std::string line{};
  std::array<std::int32_t, kBits> ones{};
  while (std::getline(std::cin, line) && !line.empty()) {
    std::bitset<kBits> bits{line};
    for (std::size_t i = 0; i < kBits; ++i) {
      if (bits[i]) {
        ++ones[i];
      }
    }
    ++count;
  }
  std::bitset<kBits> most_common_bits{};
  std::bitset<kBits> least_common_bits{};
  for (std::size_t i = 0; i < kBits; ++i) {
    if (ones[i] >= count / 2) {
      most_common_bits[i] = true;
    } else {
      least_common_bits[i] = true;
    }
  }
  const auto gamma = static_cast<std::int32_t>(most_common_bits.to_ulong());
  const auto epsilon = static_cast<std::int32_t>(least_common_bits.to_ulong());
  std::cout << gamma * epsilon << std::endl;
}
