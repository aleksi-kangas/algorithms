#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

bool IsLower(std::string_view s) {
  return std::ranges::all_of(s, [](char c) { return std::islower(c); });
}

struct Cave {
  bool is_small{false};
  std::string name{};
  std::vector<Cave*> neighbors{};
  explicit Cave(std::string name) : is_small{IsLower(name)}, name{std::move(name)} {}
};

std::unordered_map<std::string, std::unique_ptr<Cave>> ReadGraph() {
  std::unordered_map<std::string, std::unique_ptr<Cave>> g{};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    const auto hyphen_index = line.find('-');
    const std::string from_name = line.substr(0, hyphen_index);
    const std::string to_name = line.substr(hyphen_index + 1);
    Cave* from{nullptr};
    Cave* to{nullptr};
    if (const auto it = g.find(from_name); it != g.end()) {
      from = it->second.get();
    } else {
      g[from_name] = std::make_unique<Cave>(from_name);
      from = g[from_name].get();
    }
    if (const auto it = g.find(to_name); it != g.end()) {
      to = it->second.get();
    } else {
      g[to_name] = std::make_unique<Cave>(to_name);
      to = g[to_name].get();
    }
    from->neighbors.emplace_back(to);
    to->neighbors.emplace_back(from);
  }
  return g;
}

std::int32_t CountPaths(const std::unordered_set<std::string_view>& visited, const Cave* current, const Cave* target) {
  if (current == target)
    return 1;
  if (visited.contains(current->name))
    return 0;
  std::unordered_set<std::string_view> sub_visited = visited;
  if (current->is_small) {
    sub_visited.insert(current->name);
  }
  std::int32_t sub_paths{0};
  for (const auto neighbor : current->neighbors) {
    sub_paths += CountPaths(sub_visited, neighbor, target);
  }
  return sub_paths;
}

int main() {
  const auto g = ReadGraph();
  std::unordered_set<std::string_view> visited{};
  const std::int32_t paths = CountPaths(visited, g.at("start").get(), g.at("end").get());
  std::cout << paths << '\n';
}
