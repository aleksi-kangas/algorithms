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

enum class PacketType {
  Sum = 0,
  Product = 1,
  Minimum = 2,
  Maximum = 3,
  LiteralValue = 4,
  GreaterThan = 5,
  LessThan = 6,
  EqualTo = 7
};

class Packet {
 public:
  explicit Packet(const string& binary)
      : version_{bitset<3>{binary.substr(0, 3)}.to_ulong()},
        type_{static_cast<PacketType>(bitset<3>{binary.substr(3, 3)}.to_ulong())} {
    if (type_ != PacketType::LiteralValue) {
      ProcessOperatorPacket(binary);
    }
    switch (type_) {
      case PacketType::Sum: {
        unsigned long long sum = 0;
        for (const auto& sub_packet : sub_packets_) {
          sum += sub_packet.value_;
        }
        value_ = sum;
        break;
      }
      case PacketType::Product: {
        unsigned long long product = 1;
        for (const auto& sub_packet : sub_packets_) {
          product *= sub_packet.value_;
        }
        value_ = product;
        break;
      }
      case PacketType::Minimum: {
        unsigned long long minimum = numeric_limits<unsigned long long>::max();
        for (const auto& sub_packet : sub_packets_) {
          minimum = min(minimum, sub_packet.value_);
        }
        value_ = minimum;
        break;
      }
      case PacketType::Maximum: {
        unsigned long long maximum = numeric_limits<unsigned long long>::min();
        for (const auto& sub_packet : sub_packets_) {
          maximum = max(maximum, sub_packet.value_);
        }
        value_ = maximum;
        break;
      }
      case PacketType::LiteralValue: {
        string x;
        int prefix_index = 6;
        while (binary[prefix_index] == '1') {
          x += binary.substr(prefix_index + 1, 4);
          prefix_index += 5;
        }
        x += binary.substr(prefix_index + 1, 4);
        value_ = bitset<64>{x}.to_ullong();
        bits_read_ = prefix_index + 5;
        break;
      }
      case PacketType::GreaterThan: {
        value_ = sub_packets_[0].value_ > sub_packets_[1].value_;
        break;
      }
      case PacketType::LessThan: {
        value_ = sub_packets_[0].value_ < sub_packets_[1].value_;
        break;
      }
      case PacketType::EqualTo: {
        value_ = sub_packets_[0].value_ == sub_packets_[1].value_;
        break;
      }
      default: {
        assert(false);
      }
    }
  }

  [[nodiscard]] unsigned long long Value() const {
    return value_;
  }

 private:
  const unsigned long version_;
  const PacketType type_;
  unsigned long long value_ = 0;
  unsigned long bits_read_ = 0;
  vector<Packet> sub_packets_;

  void ProcessOperatorPacket(const string& binary) {
    const bool sub_packets_by_bits = bitset<1>{binary.substr(6, 1)}.to_ulong() == 0;
    if (sub_packets_by_bits) {
      const unsigned long total_bits = bitset<15>{binary.substr(7, 15)}.to_ulong();
      unsigned long offset = 0;
      while (offset < total_bits) {
        Packet sub_packet{binary.substr(22 + offset)};
        offset += sub_packet.bits_read_;
        sub_packets_.push_back(std::move(sub_packet));
      }
      bits_read_ = 22 + offset;
    } else {
      const unsigned long sub_packet_count = bitset<11>{binary.substr(7, 11)}.to_ulong();
      unsigned long offset = 0;
      for (unsigned long i = 0; i < sub_packet_count; ++i) {
        Packet sub_packet{binary.substr(18 + offset)};
        offset += sub_packet.bits_read_;
        sub_packets_.push_back(std::move(sub_packet));
      }
      bits_read_ = 18 + offset;
    }
  }
};

unsigned long Solve() {
  const string binary = ReadHexadecimalInputAsBinary();
  const Packet packet{binary};
  return packet.Value();
}

int main() {
  auto answer = Solve();
  cout << answer << endl;
}
