#include <bitset>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>
#include <vector>

// On some platforms, need to redirect input from a file:
// <executable_name> < <input_file_name>, e.g. 16_1_packet_decoder < in.txt

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
  enum class Type { kLiteral, kOperator };
  enum class Operator { kUnknown };

  static std::pair<Packet, std::size_t> Parse(const std::string& s);

  [[nodiscard]] std::uint64_t VersionSum() const;

 private:
  std::uint8_t version_{0};
  Type type_;
  std::variant<std::uint64_t, Operator> value_;
  std::vector<Packet> children_;

  Packet(std::uint8_t version, Type type, std::variant<std::uint64_t, Operator> value, std::vector<Packet> children)
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
  std::variant<std::uint64_t, Operator> value{};
  std::vector<Packet> children{};
  switch (type) {
    case Type::kLiteral: {
      const auto [literal_value, literal_bits_read] = ParseLiteral(s.substr(6));
      value = literal_value;
      bits_read += literal_bits_read;
    } break;
    case Type::kOperator: {
      value = Operator::kUnknown;
      auto [ch, children_bits_read] = ParseChildren(s.substr(6));
      children = std::move(ch);
      bits_read += children_bits_read;
    } break;
  }
  return {Packet{version, type, value, std::move(children)}, bits_read};
}

std::uint64_t Packet::VersionSum() const {
  std::uint64_t sum = version_;
  for (const Packet& child : children_) {
    sum += child.VersionSum();
  }
  return sum;
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
  return {static_cast<std::uint8_t>(type_bits.to_ulong()) == 4 ? Type::kLiteral : Type::kOperator, 3};
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
  std::cout << packet.VersionSum() << '\n';
}
