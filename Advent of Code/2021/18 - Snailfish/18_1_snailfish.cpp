#include <cstdint>
#include <iostream>
#include <memory>
#include <queue>
#include <stdexcept>
#include <string>

struct Node {
  Node* parent{nullptr};
  std::unique_ptr<Node> left{nullptr};
  std::unique_ptr<Node> right{nullptr};
  std::int32_t value{0};

  [[nodiscard]] bool HasLeft() const { return left != nullptr; }
  [[nodiscard]] bool HasRight() const { return right != nullptr; }
  [[nodiscard]] bool HasChildren() const { return HasLeft() || HasRight(); }
};

std::unique_ptr<Node> ReadNumber(std::string_view s) {
  auto root = std::make_unique<Node>();
  Node* current = root.get();
  for (const char c : s) {
    switch (c) {
      case '[': {
        current->left = std::make_unique<Node>();
        current->left->parent = current;
        current = current->left.get();
      } break;
      case ']': {
        current = current->parent;
      } break;
      case ',': {
        current->parent->right = std::make_unique<Node>();
        current->parent->right->parent = current->parent;
        current = current->parent->right.get();
      } break;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9': {
        if (current->value > 0) {
          current->value *= 10;
          current->value += c - '0';
        } else {
          current->value = c - '0';
        }
      } break;
      default:
        throw std::runtime_error{"Invalid character"};
    }
  }
  return root;
}

std::queue<std::unique_ptr<Node>> ReadNumbers() {
  std::queue<std::unique_ptr<Node>> numbers{};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    numbers.push(ReadNumber(line));
  }
  return numbers;
}

Node* FindPreviousLeaf(Node* node) {
  Node* current = node;
  // Traverse up until current becomes a right child.
  while (current->parent && current->parent->left.get() == current) {
    current = current->parent;
  }
  // Jump to the left child of the parent.
  if (!current->parent || !current->parent->left)
    return nullptr;
  current = current->parent->left.get();
  // Traverse down until the rightmost leaf.
  while (current->HasRight()) {
    current = current->right.get();
  }
  return current;
}

Node* FindNextLeaf(Node* node) {
  Node* current = node;
  // Traverse up until current becomes a left child.
  while (current->parent && current->parent->right.get() == current) {
    current = current->parent;
  }
  // Jump to the right child of the parent.
  if (!current->parent || !current->parent->right)
    return nullptr;
  current = current->parent->right.get();
  // Traverse down until the leftmost leaf.
  while (current->HasLeft()) {
    current = current->left.get();
  }
  return current;
}

bool Explode(Node* node, std::size_t depth = 0) {
  if (node->HasLeft()) {
    if (Explode(node->left.get(), depth + 1))
      return true;
  }
  if (depth >= 4) {
    const bool is_left_leaf = node->HasLeft() && !node->left->HasChildren();
    const bool is_right_left = node->HasRight() && !node->right->HasChildren();
    if (is_left_leaf && is_right_left) {
      Node* previous_leaf = FindPreviousLeaf(node);
      if (previous_leaf != nullptr) {
        previous_leaf->value += node->left->value;
      }
      Node* next_leaf = FindNextLeaf(node);
      if (next_leaf != nullptr) {
        next_leaf->value += node->right->value;
      }
      node->value = 0;
      node->left = nullptr;
      node->right = nullptr;
      return true;
    }
  }
  if (node->HasRight())
    return Explode(node->right.get(), depth + 1);
  return false;
}

bool Split(Node* node) {
  if (!node->HasChildren() && node->value >= 10) {
    node->left = std::make_unique<Node>();
    node->left->parent = node;
    node->left->value = node->value / 2;
    node->right = std::make_unique<Node>();
    node->right->parent = node;
    node->right->value = (node->value + 1) / 2;
    node->value = 0;
    return true;
  }
  if (node->HasLeft()) {
    if (Split(node->left.get()))
      return true;
    if (node->HasRight()) {
      return Split(node->right.get());
    }
  }
  return false;
}

void Reduce(Node* root) {
  bool reduced{false};
  while (!reduced) {
    // If any pair is nested inside four pairs, the leftmost such pair explodes.
    const bool exploded = Explode(root);
    if (!exploded) {
      // If any regular number is 10 or greater, the leftmost such regular number splits.
      const bool split = Split(root);
      if (!split) {
        reduced = true;
      }
    }
  }
}

std::unique_ptr<Node> Add(std::unique_ptr<Node> n1, std::unique_ptr<Node> n2) {
  auto root = std::make_unique<Node>();
  root->left = std::move(n1);
  root->left->parent = root.get();
  root->right = std::move(n2);
  root->right->parent = root.get();
  Reduce(root.get());
  return root;
}

std::int64_t Magnitude(Node* node) {
  if (!node->HasChildren())
    return node->value;
  std::int64_t magnitude{0};
  if (node->HasLeft()) {
    magnitude += 3 * Magnitude(node->left.get());
  }
  if (node->HasRight()) {
    magnitude += 2 * Magnitude(node->right.get());
  }
  return magnitude;
}

int main() {
  auto numbers = ReadNumbers();
  auto number = std::move(numbers.front());
  numbers.pop();
  while (!numbers.empty()) {
    number = Add(std::move(number), std::move(numbers.front()));
    numbers.pop();
  }
  std::cout << Magnitude(number.get()) << '\n';
}
