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

enum class Term { kOld, kNumber };
enum class OperationType { kAdd, kSubtract, kMultiply, kDivide };

function<int(int old_worry_level)> ParseOperation(const string &line) {
  string op = line.substr(line.find('=') + 2);
  stringstream ss{op};
  vector<string> tokens;
  string token;
  while (getline(ss, token, ' ')) {
    tokens.push_back(token);
  }

  Term left_term = Term::kOld;
  if (tokens[0] == "old") {
    left_term = Term::kOld;
  } else {
    left_term = Term::kNumber;
  }

  Term right_term = Term::kOld;
  if (tokens[2] == "old") {
    right_term = Term::kOld;
  } else {
    right_term = Term::kNumber;
  }

  OperationType operation_type = OperationType::kAdd;

  switch (tokens[1][0]) {
  case '+':
    operation_type = OperationType::kAdd;
    break;
  case '-':
    operation_type = OperationType::kSubtract;
    break;
  case '*':
    operation_type = OperationType::kMultiply;
    break;
  case '/':
    operation_type = OperationType::kDivide;
    break;
  default:
    assert(false);
  }

  return [=](int old_worry_level) -> int {
    int left = 0;
    int right = 0;
    switch (left_term) {
    case Term::kOld:
      left = old_worry_level;
      break;
    case Term::kNumber:
      left = stoi(tokens[0]);
      break;
    }
    switch (right_term) {
    case Term::kOld:
      right = old_worry_level;
      break;
    case Term::kNumber:
      right = stoi(tokens[2]);
      break;
    }

    switch (operation_type) {
    case OperationType::kAdd:
      return left + right;
    case OperationType::kSubtract:
      return left - right;
    case OperationType::kMultiply:
      return left * right;
    case OperationType::kDivide:
      return left / right;
    default:
      assert(false);
    }
  };
}

function<bool(int new_worry_level)> ParseTest(const string &line) {
  string test = line.substr(line.find(':') + 2);
  stringstream ss{test};
  vector<string> tokens;
  string token;
  while (getline(ss, token, ' ')) {
    tokens.push_back(token);
  }
  if (tokens[0] == "divisible") {
    int modulus = stoi(tokens[2]);
    return [=](int new_worry_level) -> bool {
      return new_worry_level % modulus == 0;
    };
  } else {
    assert(false);
  }
}

int ParseAction(const string &line) {
  string action = line.substr(line.find(':') + 2);
  stringstream ss{action};
  vector<string> tokens;
  string token;
  while (getline(ss, token, ' ')) {
    tokens.push_back(token);
  }
  int target = stoi(tokens.back());
  return target;
}

struct Monkey {
  int id = 0;
  queue<int> items;
  function<int(int old_worry_level)> operation;
  function<bool(int new_worry_level)> test;
  int on_true_target = 0;
  int on_false_target = 0;
};

Monkey ParseMonkey(const vector<string> &lines) {
  static int id = 0;
  Monkey monkey;
  monkey.id = id++;

  auto items = lines[1].substr(lines[1].find(':') + 1);
  stringstream ss(items);
  string item;
  while (getline(ss, item, ',')) {
    monkey.items.push(stoi(item));
  }

  monkey.operation = ParseOperation(lines[2]);
  monkey.test = ParseTest(lines[3]);
  monkey.on_true_target = ParseAction(lines[4]);
  monkey.on_false_target = ParseAction(lines[5]);
  return monkey;
}

vector<Monkey> ReadInput() {
  vector<Monkey> monkeys;
  string line;
  int empty_line_count = 0;
  vector<string> monkey_lines;
  while (getline(cin, line)) {
    if (line.empty()) {
      ++empty_line_count;
      if (empty_line_count >= 2)
        break;
      else
        monkeys.push_back(ParseMonkey(monkey_lines));
      monkey_lines.clear();
    } else {
      empty_line_count = 0;
      monkey_lines.push_back(line);
    }
  }
  return monkeys;
}

int Solve() {
  auto monkeys = ReadInput();

  vector<int> inspections(monkeys.size(), 0);
  for (int r = 1; r <= 20; ++r) {
    for (auto &monkey : monkeys) {
      while (!monkey.items.empty()) {
        ++inspections[monkey.id];
        int old_worry_level = monkey.items.front();
        monkey.items.pop();
        int new_worry_level = monkey.operation(old_worry_level);
        new_worry_level /= 3;
        if (monkey.test(new_worry_level)) {
          monkeys[monkey.on_true_target].items.push(new_worry_level);
        } else {
          monkeys[monkey.on_false_target].items.push(new_worry_level);
        }
      }
    }
  }
  sort(inspections.begin(), inspections.end());
  return inspections[inspections.size() - 1] *
         inspections[inspections.size() - 2];
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}