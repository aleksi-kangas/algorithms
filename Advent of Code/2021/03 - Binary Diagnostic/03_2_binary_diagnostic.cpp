#include <array>
#include <bitset>
#include <iostream>
#include <string>
#include <vector>

constexpr std::size_t kBits{12};

constexpr std::size_t IndexToBitsetIndex(std::size_t index) {
  return kBits - index - 1;
}

std::bitset<kBits> CommonBits(const std::vector<std::string_view>& binary_numbers, bool most_common) {
  std::array<std::int32_t, kBits> ones{};
  std::array<std::int32_t, kBits> zeros{};
  for (const auto& binary_number : binary_numbers) {
    for (std::size_t i = 0; i < binary_number.size(); ++i) {
      if (binary_number[i] == '1') {
        ++ones[i];
      } else {
        ++zeros[i];
      }
    }
  }
  std::bitset<kBits> common_bits{};
  for (std::size_t i = 0; i < kBits; ++i) {
    if (ones[i] > zeros[i]) {
      common_bits[IndexToBitsetIndex(i)] = most_common;
    } else if (ones[i] < zeros[i]) {
      common_bits[IndexToBitsetIndex(i)] = !most_common;
    } else {
      common_bits[IndexToBitsetIndex(i)] = most_common;
    }
  }
  return common_bits;
}

std::vector<std::string_view> FilterCandidates(const std::vector<std::string_view>& candidates, std::size_t bit_index,
                                               bool most_common) {
  const auto common_bits = CommonBits(candidates, most_common);
  std::vector<std::string_view> new_candidates{};
  for (const auto& candidate : candidates) {
    if (candidate[bit_index] - '0' != common_bits[IndexToBitsetIndex(bit_index)]) {
      continue;
    }
    new_candidates.emplace_back(candidate);
  }
  return new_candidates;
}

std::pair<std::bitset<kBits>, std::bitset<kBits>> Ratings(const std::vector<std::string>& binary_numbers) {
  std::vector<std::string_view> oxygen_generator_rating_candidates(binary_numbers.begin(), binary_numbers.end());
  std::vector<std::string_view> co2_scrubber_rating_candidates(binary_numbers.begin(), binary_numbers.end());
  const auto IsSolution = [&]() -> bool {
    return oxygen_generator_rating_candidates.size() == 1 && co2_scrubber_rating_candidates.size() == 1;
  };
  for (std::size_t i = 0; i < kBits; ++i) {
    if (oxygen_generator_rating_candidates.size() > 1) {
      oxygen_generator_rating_candidates = FilterCandidates(oxygen_generator_rating_candidates, i, true);
    }
    if (co2_scrubber_rating_candidates.size() > 1) {
      co2_scrubber_rating_candidates = FilterCandidates(co2_scrubber_rating_candidates, i, false);
    }
    if (IsSolution()) {
      break;
    }
  }
  return IsSolution() ? std::make_pair(std::bitset<kBits>{std::string{oxygen_generator_rating_candidates[0]}},
                                       std::bitset<kBits>{std::string{co2_scrubber_rating_candidates[0]}})
                      : std::make_pair(std::bitset<kBits>{}, std::bitset<kBits>{});
}

int main() {
  std::string line{};
  std::vector<std::string> binary_numbers{};
  while (std::getline(std::cin, line) && !line.empty()) {
    binary_numbers.emplace_back(line);
  }
  const auto [oxygen_generator_rating, co2_scrubber_rating] = Ratings(binary_numbers);
  std::cout << oxygen_generator_rating.to_ulong() * co2_scrubber_rating.to_ulong() << std::endl;
}
