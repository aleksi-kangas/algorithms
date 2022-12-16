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

string ReadHexadecimalInputAsBinary() {
  string s;
  cin >> s;
  string binary;
  binary.reserve(s.size() * 4);
  for (char c : s) {
    if (c >= '0' && c <= '9') {
      binary += bitset<4>(c - '0').to_string();
    } else if (c >= 'A' && c <= 'F') {
      binary += bitset<4>(c - 'A' + 10).to_string();
    } else {
      assert(false);
    }
  }
  return binary;
}

enum class PacketType { LiteralValue, Operator };

class Packet {
 public:
  explicit Packet(const string& binary)
      : version{bitset<3>{binary.substr(0, 3)}.to_ulong()},
        type{bitset<3>{binary.substr(3, 3)}.to_ulong() != 4 ? PacketType::Operator
                                                            : PacketType::LiteralValue} {
    switch (type) {
      case PacketType::LiteralValue: {
        string x;
        int prefix_index = 6;
        while (binary[prefix_index] == '1') {
          x += binary.substr(prefix_index + 1, 4);
          prefix_index += 5;
        }
        x += binary.substr(prefix_index + 1, 4);
        value_ = bitset<64>{x}.to_ullong();
        bits_read = prefix_index + 5;
        break;
      }
      case PacketType::Operator: {
        const bool sub_packets_by_bits = bitset<1>{binary.substr(6, 1)}.to_ulong() == 0;
        if (sub_packets_by_bits) {
          const unsigned long total_bits = bitset<15>{binary.substr(7, 15)}.to_ulong();
          unsigned long offset = 0;
          while (offset < total_bits) {
            Packet sub_packet{binary.substr(22 + offset)};
            offset += sub_packet.bits_read;
            sub_packets_.push_back(std::move(sub_packet));
          }
          bits_read = 22 + offset;
        } else {
          const unsigned long sub_packet_count = bitset<11>{binary.substr(7, 11)}.to_ulong();
          unsigned long offset = 0;
          for (unsigned long i = 0; i < sub_packet_count; ++i) {
            Packet sub_packet{binary.substr(18 + offset)};
            offset += sub_packet.bits_read;
            sub_packets_.push_back(std::move(sub_packet));
          }
          bits_read = 18 + offset;
        }
        break;
      }
      default: {
        assert(false);
      }
    }
  }

  const unsigned long version;
  const PacketType type;
  unsigned long bits_read = 0;

  [[nodiscard]] unsigned long AddPacketVersions() const {
    unsigned long sum = version;
    for (const Packet& p : sub_packets_) {
      sum += p.AddPacketVersions();
    }
    return sum;
  }

 private:
  unsigned long long value_;
  vector<Packet> sub_packets_;
};

unsigned long Solve() {
  const string binary = ReadHexadecimalInputAsBinary();
  const Packet packet{binary};
  return packet.AddPacketVersions();
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}
