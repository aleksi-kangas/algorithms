#include <algorithm>
#include <bitset>
#include <cstdint>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// On some platforms, need to redirect input from a file:
// <executable_name> < <input_file_name>, e.g. 16_2_packet_decoder < in.txt


std::string HexadecimalToBinary(std::string_view hexadecimal) {
  std::string binary{};
  binary.reserve(hexadecimal.size() * 4);
  for (const char c : hexadecimal) {
    if ('0' <= c && c <= '9') {
      binary += std::bitset<4>(c - '0').to_string();
    } else if ('A' <= c && c <= 'F') {
      binary += std::bitset<4>(c - 'A' + 10).to_string();
    } else {
      throw std::runtime_error{"Invalid hexadecimal"};
    }
  }
  return binary;
}

class Packet {
 public:
  enum class Type {
    kSum = 0,
    kProduct = 1,
    kMinimum = 2,
    kMaximum = 3,
    kLiteral = 4,
    kGreaterThan = 5,
    kLessThan = 6,
    kEqualTo = 7
  };

  static std::pair<Packet, std::size_t> Parse(const std::string& s);

  [[nodiscard]] std::uint64_t Value() const;

 private:
  std::uint8_t version_{0};
  Type type_{Type::kLiteral};
  std::uint64_t value_{0};
  std::vector<Packet> children_;

  Packet(std::uint8_t version, Type type, std::uint64_t value, std::vector<Packet> children)
      : version_{version}, type_{type}, value_{value}, children_{std::move(children)} {}

  static std::pair<std::uint8_t, std::size_t> ParseVersion(const std::string& s);
  static std::pair<Type, std::size_t> ParseType(const std::string& s);
  static std::pair<std::uint64_t, std::size_t> ParseLiteral(std::string_view s);
  static std::pair<std::vector<Packet>, std::size_t> ParseChildren(const std::string& s);
};

std::pair<Packet, std::size_t> Packet::Parse(const std::string& s) {
  std::size_t bits_read{0};
  const auto [version, version_bits_read] = ParseVersion(s.substr(0, 3));
  bits_read += version_bits_read;
  const auto [type, type_bits_read] = ParseType(s.substr(3, 3));
  bits_read += type_bits_read;
  std::uint64_t value{0};
  std::vector<Packet> children{};
  auto OperatorCommon = [&]() {
    auto [ch, children_bits_read] = ParseChildren(s.substr(6));
    children = std::move(ch);
    bits_read += children_bits_read;
  };
  switch (type) {
    case Type::kSum: {
      OperatorCommon();
      value = 0;
      for (const Packet& child : children) {
        value += child.value_;
      }
    } break;
    case Type::kProduct: {
      OperatorCommon();
      value = 1;
      for (const Packet& child : children) {
        value *= child.value_;
      }
    } break;
    case Type::kMinimum: {
      OperatorCommon();
      value = std::numeric_limits<std::uint64_t>::max();
      for (const Packet& child : children) {
        value = std::min(value, child.value_);
      }
    } break;
    case Type::kMaximum: {
      OperatorCommon();
      value = std::numeric_limits<std::uint64_t>::min();
      for (const Packet& child : children) {
        value = std::max(value, child.value_);
      }
    } break;
    case Type::kLiteral: {
      const auto [literal_value, literal_bits_read] = ParseLiteral(s.substr(6));
      value = literal_value;
      bits_read += literal_bits_read;
    } break;
    case Type::kGreaterThan: {
      OperatorCommon();
      if (children.size() != 2)
        throw std::runtime_error{"Invalid operator"};
      value = children[0].value_ > children[1].value_;

    } break;
    case Type::kLessThan: {
      OperatorCommon();
      if (children.size() != 2)
        throw std::runtime_error{"Invalid operator"};
      value = children[0].value_ < children[1].value_;
    } break;
    case Type::kEqualTo: {
      OperatorCommon();
      if (children.size() != 2)
        throw std::runtime_error{"Invalid operator"};
      value = children[0].value_ == children[1].value_;
    } break;
  }
  return {Packet{version, type, value, std::move(children)}, bits_read};
}

std::uint64_t Packet::Value() const {
  return value_;
}

std::pair<std::uint8_t, std::size_t> Packet::ParseVersion(const std::string& s) {
  if (s.size() != 3)
    throw std::runtime_error{"Invalid version"};
  const std::bitset<3> version_bits{s};
  return {static_cast<std::uint8_t>(version_bits.to_ulong()), 3};
}

std::pair<Packet::Type, std::size_t> Packet::ParseType(const std::string& s) {
  if (s.size() != 3)
    throw std::runtime_error{"Invalid type"};
  const std::bitset<3> type_bits{s};
  return {static_cast<Packet::Type>(type_bits.to_ulong()), 3};
}

std::pair<std::uint64_t, std::size_t> Packet::ParseLiteral(std::string_view s) {
  constexpr std::size_t kBits{5};
  std::size_t part{0};
  std::string binary{};
  while (s[part * kBits] == '1') {
    binary += s.substr(part * kBits + 1, kBits - 1);
    ++part;
  }
  binary += s.substr(part * kBits + 1, kBits - 1);
  return {std::bitset<64>(binary).to_ullong(), (part + 1) * kBits};
}

std::pair<std::vector<Packet>, std::size_t> Packet::ParseChildren(const std::string& s) {
  switch (s[0]) {
    case '0': {  // Next 15 bits represent the total length in bits of the sub-packets
      const std::size_t total_length_bits = std::bitset<15>{s.substr(1, 15)}.to_ulong();
      std::size_t bits_read{16};
      std::size_t offset{16};
      std::vector<Packet> children{};
      while (bits_read - 16 < total_length_bits) {
        auto [child, child_bits_read] = Packet::Parse(s.substr(offset));
        children.emplace_back(std::move(child));
        offset += child_bits_read;
        bits_read += child_bits_read;
      }
      return {children, bits_read};
    }
    case '1': {  // Next 11 bits represent the number of sub-packets
      const std::size_t sub_packet_count = std::bitset<11>{s.substr(1, 11)}.to_ulong();
      std::size_t bits_read{12};
      std::size_t offset{12};
      std::vector<Packet> children{};
      for (std::size_t i = 0; i < sub_packet_count; ++i) {
        auto [child, child_bits_read] = Packet::Parse(s.substr(offset));
        children.emplace_back(std::move(child));
        offset += child_bits_read;
        bits_read += child_bits_read;
      }
      return {children, bits_read};
    }
    default:
      throw std::runtime_error{"Invalid operator"};
  }
}

int main() {
  std::string hexadecimal{};
  std::cin >> hexadecimal;
  const std::string binary = HexadecimalToBinary(hexadecimal);
  const auto [packet, bits_read] = Packet::Parse(binary);
  std::cout << packet.Value() << '\n';
}
