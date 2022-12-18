#include <algorithm>
#include <bitset>
#include <cassert>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

using namespace std;

using ll = long long;

struct Node {
  int value = -1;
  shared_ptr<Node> parent = nullptr;
  shared_ptr<Node> left = nullptr;
  shared_ptr<Node> right = nullptr;
  bool IsLeaf() { return !left && !right; }
};

shared_ptr<Node> ParseNumber(const string& s) {
  auto root = make_shared<Node>();

  shared_ptr<Node> current = root;
  for (char c : s) {
    if (c == '[') {
      auto node = make_shared<Node>();
      node->parent = current;
      current->left = node;
      current = node;
    } else if (c == ']') {
      current = current->parent;
    } else if (c == ',') {
      current = current->parent;
      auto node = make_shared<Node>();
      node->parent = current;
      current->right = node;
      current = node;
    } else if (isdigit(c)) {
      if (current->value > 0) {
        current->value *= 10;
        current->value += c - '0';
      } else {
        current->value = c - '0';
      }
    } else {
      assert(false);
    }
  }
  return root;
}

void PrintNumber(Node* root) {
  if (root->IsLeaf()) {
    cout << root->value;
  } else {
    cout << '[';
    PrintNumber(root->left.get());
    cout << ',';
    PrintNumber(root->right.get());
    cout << ']';
  }
}

vector<shared_ptr<Node>> ParseNumbers() {
  vector<shared_ptr<Node>> numbers;
  string s;
  while (getline(cin, s) && !s.empty()) {
    numbers.push_back(ParseNumber(s));
  }
  return numbers;
}

Node* FindPreviousLeaf(Node* node) {
  Node* current = node;
  // Walk up until current is a right child
  while (current->parent && current->parent->left.get() == current) {
    current = current->parent.get();
  }
  // Jump to the parent's left child
  if (!current->parent || !current->parent->left) return nullptr;
  current = current->parent->left.get();
  // Walk down-right until current is a leaf
  while (current->right) {
    current = current->right.get();
  }
  return current;
}

Node* FindNextLeaf(Node* node) {
  Node* current = node;
  // Walk up until current is a left child
  while (current->parent && current->parent->right.get() == current) {
    current = current->parent.get();
  }
  // Jump to the parent's right child
  if (!current->parent || !current->parent->right) {
    return nullptr;
  }
  current = current->parent->right.get();
  // Walk down-left until current is a leaf
  while (current->left) {
    current = current->left.get();
  }
  return current;
}

bool Explode(Node* node, int depth) {
  bool exploded = false;
  if (node->left) {
    exploded |= Explode(node->left.get(), depth + 1);
  }
  const bool is_left_leaf = node->left && node->left->IsLeaf();
  const bool is_right_leaf = node->right && node->right->IsLeaf();
  if (is_left_leaf && is_right_leaf && depth >= 4) {
    Node* previous_leaf = FindPreviousLeaf(node);
    if (previous_leaf) {
      previous_leaf->value += node->left->value;
    }
    Node* next_leaf = FindNextLeaf(node);
    if (next_leaf) {
      next_leaf->value += node->right->value;
    }
    node->value = 0;
    node->left = nullptr;
    node->right = nullptr;
    exploded = true;
  }
  if (!exploded && node->right) {
    exploded |= Explode(node->right.get(), depth + 1);
  }
  return exploded;
}

bool Split(const shared_ptr<Node>& node) {
  bool split = false;
  if (node->left) {
    split |= Split(node->left);
  }
  if (node->IsLeaf() && node->value >= 10) {
    auto left = make_shared<Node>();
    left->value = floor(node->value / 2.0);
    left->parent = node;
    node->left = left;
    auto right = make_shared<Node>();
    right->value = ceil(node->value / 2.0);
    right->parent = node;
    node->right = right;
    node->value = -1;
    split = true;
  }
  if (!split && node->right) {
    split |= Split(node->right);
  }
  return split;
}

void Reduce(const shared_ptr<Node>& root) {
  while (true) {
    bool reduced = Explode(root.get(), 0);
    if (!reduced) {
      reduced = Split(root);
    }
    if (!reduced) break;
  }
}

shared_ptr<Node> Add(shared_ptr<Node> n1, shared_ptr<Node> n2) {
  auto root = make_shared<Node>();
  n1->parent = root;
  root->left = std::move(n1);
  n2->parent = root;
  root->right = std::move(n2);
  Reduce(root);
  return root;
}

int Magnitude(Node* node) {
  if (node->IsLeaf()) {
    return node->value;
  }
  int left_magnitude = 0;
  if (node->left) {
    left_magnitude = Magnitude(node->left.get());
  }
  int right_magnitude = 0;
  if (node->right) {
    right_magnitude = Magnitude(node->right.get());
  }
  return 3 * left_magnitude + 2 * right_magnitude;
}

int Solve() {
  vector<shared_ptr<Node>> numbers = ParseNumbers();
  shared_ptr<Node> n1 = numbers[0];
  for (auto i = 1; i < numbers.size(); ++i) {
    n1 = Add(n1, numbers[i]);
  }
  return Magnitude(n1.get());
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}
