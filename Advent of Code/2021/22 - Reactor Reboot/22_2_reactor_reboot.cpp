#include <algorithm>
#include <cstdint>
#include <iostream>
#include <list>
#include <optional>
#include <ranges>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

struct AABB {
  std::pair<std::int64_t, std::int64_t> x{};
  std::pair<std::int64_t, std::int64_t> y{};
  std::pair<std::int64_t, std::int64_t> z{};
  bool on{true};

  AABB(std::int64_t x1, std::int64_t x2, std::int64_t y1, std::int64_t y2, std::int64_t z1, std::int64_t z2, bool on)
      : x{x1, x2}, y{y1, y2}, z{z1, z2}, on{on} {
    if (x1 > x2 || y1 > y2 || z1 > z2) {
      throw std::invalid_argument{"Invalid AABB"};
    }
  }

  [[nodiscard]] std::uint64_t Volume() const {
    return (x.second - x.first + 1) * (y.second - y.first + 1) * (z.second - z.first + 1);
  }

  [[nodiscard]] std::uint64_t IntersectionVolume(const std::list<AABB>& aabbs) const {
    std::uint64_t intersection_volume{0};
    for (auto it = aabbs.begin(); it != aabbs.end(); ++it) {
      const auto& aabb = *it;
      auto intersection = Intersection(*this, aabb);
      if (intersection.has_value()) {
        auto next = std::next(it);
        intersection_volume += intersection->Volume() - intersection->IntersectionVolume({next, aabbs.end()});
      }
    }
    return intersection_volume;
  }

  static std::optional<AABB> Intersection(const AABB& aabb1, const AABB& aabb2) {
    const std::int64_t x1 = std::max(aabb1.x.first, aabb2.x.first);
    const std::int64_t x2 = std::min(aabb1.x.second, aabb2.x.second);
    const std::int64_t y1 = std::max(aabb1.y.first, aabb2.y.first);
    const std::int64_t y2 = std::min(aabb1.y.second, aabb2.y.second);
    const std::int64_t z1 = std::max(aabb1.z.first, aabb2.z.first);
    const std::int64_t z2 = std::min(aabb1.z.second, aabb2.z.second);
    if (x1 > x2 || y1 > y2 || z1 > z2)
      return std::nullopt;
    return AABB{x1, x2, y1, y2, z1, z2, true};
  }
};

std::vector<AABB> ReadAABBs() {
  std::vector<AABB> aabbs{};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    const bool on = line.starts_with("on");
    const auto x_index = line.find("x=");
    const auto x_separator_index = line.find("..");
    const auto y_index = line.find(",y=");
    const auto y_separator_index = line.find("..", y_index);
    const auto z_index = line.find(",z=");
    const auto z_separator_index = line.find("..", z_index);
    const auto x1 = std::stoi(line.substr(x_index + 2, x_separator_index - x_index - 2));
    const auto x2 = std::stoi(line.substr(x_separator_index + 2, y_index - x_separator_index - 2));
    const auto y1 = std::stoi(line.substr(y_index + 3, y_separator_index - y_index - 3));
    const auto y2 = std::stoi(line.substr(y_separator_index + 2, z_index - y_separator_index - 2));
    const auto z1 = std::stoi(line.substr(z_index + 3));
    const auto z2 = std::stoi(line.substr(z_separator_index + 2));
    aabbs.emplace_back(x1, x2, y1, y2, z1, z2, on);
  }
  return aabbs;
}

int main() {
  const auto aabbs = ReadAABBs();
  // Iterate in inverse order and for each 'on' AABB:
  // - Add the AABB volume to the total volume.
  // - Subtract the intersection volume with the previously seen AABBs.
  // - Due to the inverse order, seen 'off' and 'on' AABBs behave similarly.
  std::uint64_t on_volume{0};
  std::list<AABB> seen{};
  for (const auto& aabb : std::ranges::reverse_view(aabbs)) {
    if (aabb.on) {
      on_volume += aabb.Volume() - aabb.IntersectionVolume(seen);
    }
    seen.push_front(aabb);
  }
  std::cout << on_volume << '\n';
}
