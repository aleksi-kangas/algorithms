#include <deque>
#include <iostream>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

using cache_key_t = std::pair<std::string, std::size_t>;

template <>
struct std::hash<cache_key_t> {
  std::size_t operator()(const cache_key_t& ck) const noexcept {
    const std::size_t h1 = std::hash<std::string>{}(ck.first);
    const std::size_t h2 = std::hash<std::size_t>{}(ck.second);
    return h1 ^ (h2 << 1);
  }
};

constexpr std::size_t kMultiplier{5};

std::string ReadSprings(std::string_view line) {
  const auto space_position = line.find(' ');
  if (space_position == std::string_view::npos)
    throw std::invalid_argument{"Space not found"};
  std::string springs{};
  for (std::size_t i{1}; i < kMultiplier; ++i) {
    springs += line.substr(0, space_position);
    springs += '?';
  }
  springs += line.substr(0, space_position);
  return springs;
}

std::deque<std::size_t> ReadGroups(std::string_view line) {
  const auto space_position = line.find(' ');
  if (space_position == std::string_view::npos)
    throw std::invalid_argument{"Space not found"};
  std::deque<std::size_t> groups{};
  std::istringstream iss{std::string{line.substr(space_position + 1)}};
  std::string segment{};
  while (std::getline(iss, segment, ',')) {
    groups.push_back(std::stoul(segment));
  }
  const std::size_t original_size = groups.size();
  for (std::size_t i{1}; i < kMultiplier; ++i) {
    for (std::size_t j{0}; j < original_size; ++j) {
      groups.push_back(groups[j]);
    }
  }
  return groups;
}

bool ContinueGroup(std::string& springs, std::size_t group_size) {
  if (springs.empty() || springs.front() == '.' || group_size > springs.size())
    return false;
  const std::size_t group_end{group_size - 1};
  for (std::size_t i{0}; i <= group_end; ++i) {
    if (springs[i] == '.')
      return false;
    springs[i] = '#';
  }
  if (group_end == springs.size() - 1)
    return true;
  if (springs[group_end + 1] == '#')
    return false;
  springs[group_end + 1] = '.';
  return true;
}

std::size_t ArrangementCount(const std::string& springs, std::deque<std::size_t>& groups,
                             std::unordered_map<cache_key_t, std::size_t>& cache) {
  const auto cache_it = cache.find({springs, groups.size()});
  if (cache_it != cache.end())
    return cache_it->second;
  if (groups.empty())
    return std::ranges::all_of(springs, [](const char c) { return c != '#'; }) ? 1 : 0;
  if (springs.empty())
    return 0;
  std::size_t count{0};
  switch (springs.front()) {
    case '#': {
      std::string damaged{springs};
      if (ContinueGroup(damaged, groups.front())) {
        const std::size_t group_size = groups.front();
        groups.pop_front();
        count += ArrangementCount(damaged.substr(group_size), groups, cache);
        groups.push_front(group_size);
      }
    } break;
    case '?': {
      const std::size_t group_size = groups.front();
      // Damaged
      std::string damaged{springs};
      if (ContinueGroup(damaged, group_size)) {
        groups.pop_front();
        count += ArrangementCount(damaged.substr(group_size), groups, cache);
        groups.push_front(group_size);
      }
      // Operational
      count += ArrangementCount(springs.substr(1), groups, cache);
    } break;
    case '.': {
      count += ArrangementCount(springs.substr(1), groups, cache);
    } break;
    default:
      throw std::runtime_error{"Unexpected character"};
  }
  cache[{springs, groups.size()}] = count;
  return count;
}

int main() {
  std::size_t arrangement_count{0};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    const auto springs = ReadSprings(line);
    auto groups = ReadGroups(line);
    std::unordered_map<cache_key_t, std::size_t> cache{};
    arrangement_count += ArrangementCount(springs, groups, cache);
  }
  std::cout << arrangement_count << std::endl;
}
