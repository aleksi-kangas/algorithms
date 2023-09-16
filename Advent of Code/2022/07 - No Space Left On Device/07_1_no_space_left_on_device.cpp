#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

enum class Command { kNone, kCD, kLS };

struct File {
  std::string name{};
  std::uint64_t size{};

  File(std::string name, std::uint64_t size) : name{std::move(name)}, size{size} {}
};

struct Directory {
  std::string name{};
  Directory* parent{nullptr};
  std::unordered_map<std::string, std::unique_ptr<Directory>> subdirectories{};
  std::vector<File> files{};

  Directory(std::string name, Directory* parent) : name{std::move(name)}, parent{parent} {}

  [[nodiscard]] std::uint64_t Size() const {
    std::uint64_t size{0};
    for (const auto& [_, subdirectory] : subdirectories) {
      size += subdirectory->Size();
    }
    for (const auto& file : files) {
      size += file.size;
    }
    return size;
  }
};

std::vector<std::string_view> Split(std::string_view s, char c) {
  std::vector<std::string_view> result{};
  std::size_t start = 0;
  std::size_t end = s.find(c);
  while (end != std::string_view::npos) {
    result.push_back(s.substr(start, end - start));
    start = end + 1;
    end = s.find(c, start);
  }
  result.push_back(s.substr(start, end));
  return result;
}

bool IsCommand(std::string_view s) {
  return s.starts_with("$");
}

Command ParseCommand(std::string_view line) {
  const auto line_parts = Split(line, ' ');
  if (line_parts[1] == "cd") {
    return Command::kCD;
  } else if (line_parts[1] == "ls") {
    return Command::kLS;
  }
  return Command::kNone;
}

std::unique_ptr<Directory> ParseFilesystemTree() {
  std::unique_ptr<Directory> root{nullptr};
  std::string line{};
  Directory* current{nullptr};
  Command command{Command::kNone};
  while (std::getline(std::cin, line) && !line.empty()) {
    const auto line_parts = Split(line, ' ');
    if (IsCommand(line)) {
      if (line_parts[1] == "cd") {
        if (line_parts[2] == "/") {
          if (root == nullptr) {
            root = std::make_unique<Directory>("/", nullptr);
            current = root.get();
          } else {
            current = root.get();
          }
        } else if (line_parts[2] == "..") {
          if (current->parent != nullptr) {
            current = current->parent;
          }
        } else {
          current = current->subdirectories[std::string{line_parts[2]}].get();
        }
      }
    } else {
      if (line_parts.size() != 2)
        throw std::runtime_error{"Invalid line"};
      if (line_parts[0] == "dir") {
        current->subdirectories[std::string{line_parts[1]}] =
            std::make_unique<Directory>(std::string{line_parts[1]}, current);
      } else {
        current->files.emplace_back(std::string{line_parts[1]}, std::stoull(std::string{line_parts[0]}));
      }
    }
  }
  return root;
}

std::uint64_t SumOfDirectorySizes(const Directory* root, std::uint64_t size_constraint, bool less_than, bool equal) {
  std::uint64_t sum{0};
  for (const auto& [_, subdirectory] : root->subdirectories) {
    const std::uint64_t subdirectory_size = subdirectory->Size();
    if (less_than && equal && subdirectory_size <= size_constraint) {
      sum += subdirectory_size;
    }
    if (less_than && !equal && subdirectory_size < size_constraint) {
      sum += subdirectory_size;
    }
    if (!less_than && equal && subdirectory_size >= size_constraint) {
      sum += subdirectory_size;
    }
    if (!less_than && !equal && subdirectory_size > size_constraint) {
      sum += subdirectory_size;
    }
    sum += SumOfDirectorySizes(subdirectory.get(), size_constraint, less_than, equal);
  }
  return sum;
}

int main() {
  const auto root = ParseFilesystemTree();
  const std::uint64_t answer = SumOfDirectorySizes(root.get(), 100000, true, true);
  std::cout << answer << std::endl;
}
