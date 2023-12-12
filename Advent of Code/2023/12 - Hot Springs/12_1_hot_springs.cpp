#include <deque>
#include <iostream>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>

std::string ReadSprings(std::string_view line) {
  const auto space_position = line.find(' ');
  if (space_position == std::string_view::npos)
    throw std::invalid_argument{"Space not found"};
  return std::string{line.substr(0, space_position)};
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

std::size_t ArrangementCount(const std::string& springs, std::deque<std::size_t>& groups) {
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
        count += ArrangementCount(damaged.substr(group_size), groups);
        groups.push_front(group_size);
      }
    } break;
    case '?': {
      const std::size_t group_size = groups.front();
      // Damaged
      std::string damaged{springs};
      if (ContinueGroup(damaged, group_size)) {
        groups.pop_front();
        count += ArrangementCount(damaged.substr(group_size), groups);
        groups.push_front(group_size);
      }
      // Operational
      count += ArrangementCount(springs.substr(1), groups);
    } break;
    case '.': {
      count += ArrangementCount(springs.substr(1), groups);
    } break;
    default:
      throw std::runtime_error{"Unexpected character"};
  }
  return count;
}

int main() {
  std::size_t arrangement_count{0};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    const auto springs = ReadSprings(line);
    auto groups = ReadGroups(line);
    arrangement_count += ArrangementCount(springs, groups);
  }
  std::cout << arrangement_count << std::endl;
}
