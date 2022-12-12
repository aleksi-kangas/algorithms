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

vector<vector<char>> ReadGrid(pair<int, int> &start, pair<int, int> &end) {
  vector<vector<char>> grid;
  string line;
  while (getline(cin, line) && !line.empty()) {
    vector<char> row;
    for (char c : line) {
      if (c == 'S') {
        start = {grid.size(), row.size()};
        row.push_back('a');
      } else if (c == 'E') {
        end = {grid.size(), row.size()};
        row.push_back('z');
      } else {
        row.push_back(c);
      }
    }
    grid.push_back(row);
  }
  return grid;
}

struct Node {
  vector<shared_ptr<Node>> neighbors;
  int row = 0;
  int column = 0;
  int steps = 0;
};

vector<vector<shared_ptr<Node>>>
BuildGridGraph(const vector<vector<char>> &grid) {
  int rows = static_cast<int>(grid.size());
  int cols = static_cast<int>(grid[0].size());
  vector<vector<shared_ptr<Node>>> graph(rows, vector<shared_ptr<Node>>(cols));
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      graph[r][c] = make_shared<Node>();
      graph[r][c]->row = r;
      graph[r][c]->column = c;
    }
  }

  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      if (0 < r && grid[r][c] - grid[r - 1][c] >= -1) {
        graph[r][c]->neighbors.push_back(graph[r - 1][c]);
      }
      if (r < rows - 1 && grid[r][c] - grid[r + 1][c] >= -1) {
        graph[r][c]->neighbors.push_back(graph[r + 1][c]);
      }
      if (0 < c && grid[r][c] - grid[r][c - 1] >= -1) {
        graph[r][c]->neighbors.push_back(graph[r][c - 1]);
      }
      if (c < cols - 1 && grid[r][c] - grid[r][c + 1] >= -1) {
        graph[r][c]->neighbors.push_back(graph[r][c + 1]);
      }
    }
  }
  return graph;
}

void BFS(vector<vector<shared_ptr<Node>>> &graph, const pair<int, int> &start) {
  vector<vector<bool>> visited(graph.size(),
                               vector<bool>(graph[0].size(), false));

  queue<shared_ptr<Node>> q;
  q.push(graph[start.first][start.second]);
  visited[start.first][start.second] = true;

  while (!q.empty()) {
    auto node = q.front();
    q.pop();
    for (const auto &neighbor : node->neighbors) {
      if (!visited[neighbor->row][neighbor->column]) {
        visited[neighbor->row][neighbor->column] = true;
        neighbor->steps = node->steps + 1;
        q.push(neighbor);
      }
    }
  }
}

int Solve() {
  pair<int, int> start, end;
  const auto grid = ReadGrid(start, end);
  auto graph = BuildGridGraph(grid);

  BFS(graph, start);
  return graph[end.first][end.second]->steps;
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}