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

using SensorBeaconPair = pair<pair<int, int>, pair<int, int>>;

vector<SensorBeaconPair> ReadInput() {
  vector<SensorBeaconPair> result;
  string line;
  while (getline(cin, line) && !line.empty()) {
    const auto first_equals_index = line.find('=');
    const auto first_comma_index = line.find(',');
    const auto second_equals_index = line.find('=', first_equals_index + 1);
    const auto first_colon_index = line.find(':');
    const auto third_equals_index = line.find('=', second_equals_index + 1);
    const auto second_comma_index = line.find(',', first_comma_index + 1);
    const auto fourth_equals_index = line.find('=', third_equals_index + 1);
    const auto second_colon_index = line.find(':', first_colon_index + 1);

    const auto sensor_x = stoi(line.substr(
        first_equals_index + 1, first_comma_index - first_equals_index - 1));
    const auto sensor_y = stoi(line.substr(
        second_equals_index + 1, first_colon_index - second_equals_index - 1));
    const auto beacon_x = stoi(line.substr(
        third_equals_index + 1, second_comma_index - third_equals_index - 1));
    const auto beacon_y = stoi(line.substr(
        fourth_equals_index + 1, second_colon_index - fourth_equals_index - 1));
    result.emplace_back(make_pair(sensor_x, sensor_y),
                        make_pair(beacon_x, beacon_y));
  }

  return result;
}

int Solve() {
  constexpr int kRow = 2000000;

  const auto sensor_beacon_pairs = ReadInput();

  set<pair<int, int>> overlap_ranges;
  for (const auto &[sensor, beacon] : sensor_beacon_pairs) {
    const auto [sensor_x, sensor_y] = sensor;
    const auto [beacon_x, beacon_y] = beacon;
    // Manhattan distance -> radius is easy to compute
    const auto radius = abs(sensor_x - beacon_x) + abs(sensor_y - beacon_y);
    const auto row_offset = abs(sensor_y - kRow);
    if (row_offset > radius)
      continue;
    const auto left = sensor_x - (radius - row_offset);
    const auto right = sensor_x + (radius - row_offset);
    overlap_ranges.emplace(left, right);
  }
  unordered_set<int> covered;
  for (const auto &[left, right] : overlap_ranges) {
    for (int i = left; i <= right; ++i) {
      covered.insert(i);
    }
  }
  for (const auto &[sensor, beacon] : sensor_beacon_pairs) {
    if (sensor.second == kRow)
      covered.erase(sensor.first);
    if (beacon.second == kRow)
      covered.erase(beacon.first);
  }
  return static_cast<int>(covered.size());
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}