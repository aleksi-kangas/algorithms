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
#include <vector>

using namespace std;

using ll = long long;

struct File {
  string name;
  int size;
};

struct Directory {
  string name;
  shared_ptr<Directory> parent;
  unordered_map<string, shared_ptr<Directory>> children;
  std::vector<File> files;
};

vector<string> Split(const string &s, char c) {
  vector<string> result;
  stringstream ss(s);
  string item;
  while (getline(ss, item, c)) {
    result.push_back(item);
  }
  return result;
}

bool IsCommand(const string &s) { return s[0] == '$'; }

void BuildFilesystemTree(shared_ptr<Directory> root,
                         const vector<string> &lines) {
  auto current = root;
  for (const auto &line : lines) {
    vector<string> parts = Split(line, ' ');
    if (IsCommand(parts[0])) {
      if (parts[1] == "cd") {
        if (parts[2] == "..") {
          current = current->parent;
        } else if (parts[2] == "/") {
          current = root;
        } else {
          current = current->children[parts[2]];
        }
      } else if (parts[1] == "ls") {
        continue;
      } else {
        throw runtime_error{"Unknown command: " + parts[1]};
      }
    } else {
      if (parts[0] == "dir") {
        auto child = make_shared<Directory>();
        child->parent = current;
        child->name = parts[1];
        current->children[child->name] = child;
      } else {
        File file{.name = parts[1], .size = stoi(parts[0])};
        current->files.push_back(file);
      }
    }
  }
}

ll DirectorySize(shared_ptr<Directory> directory) {
  ll sum = 0;
  for (const auto &file : directory->files) {
    sum += file.size;
  }
  for (const auto &[_, child] : directory->children) {
    sum += DirectorySize(child);
  }
  return sum;
}

void WalkTree(shared_ptr<Directory> directory, ll &size) {
  ll directory_size = DirectorySize(directory);
  if (directory_size <= 100000) {
    size += directory_size;
  }
  for (const auto &[_, child] : directory->children) {
    WalkTree(child, size);
  }
}

ll Solve() {
  vector<string> lines;
  string line;
  while (getline(cin, line) && !line.empty()) {
    lines.push_back(line);
  }
  auto root = make_shared<Directory>();
  BuildFilesystemTree(root, lines);
  ll total_size = 0;
  WalkTree(root, total_size);
  return total_size;
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}