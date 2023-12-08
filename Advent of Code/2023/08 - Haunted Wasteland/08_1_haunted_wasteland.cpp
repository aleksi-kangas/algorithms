#include <iostream>
#include <istream>
#include <queue>
#include <ranges>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

enum class Direction { kLeft, kRight };

struct Node {
  std::string id{};
  std::pair<Node*, Node*> children{};

  [[nodiscard]] bool IsStart() const { return id == "AAA"; }

  [[nodiscard]] bool IsEnd() const { return id == "ZZZ"; }

  [[nodiscard]] static Node Parse(std::string_view line) {
    if (line.size() < 16)
      throw std::invalid_argument{"Invalid line"};
    Node node{};
    node.id = line.substr(0, 3);
    node.children_names.first = line.substr(7, 3);
    node.children_names.second = line.substr(12, 3);
    return node;
  }

  [[nodiscard]] static std::vector<Node> ParseNodes(std::istream& is) {
    std::vector<Node> nodes{};
    std::string line{};
    while (std::getline(is, line) && !line.empty()) {
      auto node = Node::Parse(line);
      nodes.push_back(node);
    }
    for (auto& node : nodes) {
      auto left_it = std::ranges::find_if(nodes, [&node](const auto& n) { return n.id == node.children_names.first; });
      auto right_it =
          std::ranges::find_if(nodes, [&node](const auto& n) { return n.id == node.children_names.second; });
      if (left_it == nodes.end() || right_it == nodes.end())
        throw std::runtime_error{"Invalid node"};
      node.children.first = &*left_it;
      node.children.second = &*right_it;
    }
    return nodes;
  }

 private:
  std::pair<std::string, std::string> children_names{};
};

std::queue<Direction> ParseInstructions(std::istream& is) {
  std::string line{};
  std::getline(is, line);
  std::queue<Direction> instructions{};
  std::ranges::for_each(line, [&instructions](const auto c) {
    switch (c) {
      case 'L':
        instructions.push(Direction::kLeft);
        break;
      case 'R':
        instructions.push(Direction::kRight);
        break;
      default:
        throw std::invalid_argument{"Invalid instruction"};
    }
  });
  std::getline(is, line);  // Ignore the empty line
  return instructions;
}

std::int32_t StepsToReachEnd(const Node* node, std::queue<Direction> instructions) {
  std::int32_t steps{0};
  while (!node->IsEnd()) {
    auto instruction = instructions.front();
    instructions.pop();
    switch (instruction) {
      case Direction::kLeft:
        node = node->children.first;
        break;
      case Direction::kRight:
        node = node->children.second;
        break;
      default:
        throw std::logic_error{"Invalid instruction"};
    }
    instructions.push(instruction);
    ++steps;
  }
  return steps;
}

int main() {
  const auto instructions = ParseInstructions(std::cin);
  const auto nodes = Node::ParseNodes(std::cin);
  const auto start_node_it = std::ranges::find_if(nodes, [](const auto& node) { return node.IsStart(); });
  if (start_node_it == nodes.end())
    throw std::runtime_error{"Invalid nodes"};
  std::cout << StepsToReachEnd(&*start_node_it, instructions) << std::endl;
}
