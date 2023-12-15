#include <cstdint>
#include <iostream>
#include <istream>
#include <list>
#include <optional>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// On some platforms, need to redirect input from a file:
// ./<executable_name> < <input_file_name>, e.g. ./15_2_lens_library < in.txt

struct Lens {
  std::string label{};
  std::optional<std::size_t> focal_length{std::nullopt};

  [[nodiscard]] std::size_t Box() const;

  [[nodiscard]] std::size_t FocusingPower(std::size_t slot) const;

  [[nodiscard]] static Lens Parse(std::string_view s);

  bool operator==(const Lens& other) const;

 private:
  [[nodiscard]] static std::size_t Hash(std::string_view s);
};

std::size_t Lens::Box() const {
  return Hash(label);
}

std::size_t Lens::FocusingPower(std::size_t slot) const {
  return (1 + Box()) * slot * focal_length.value_or(1);
}

Lens Lens::Parse(std::string_view s) {
  auto operation_position = s.find('-');
  if (operation_position == std::string_view::npos) {
    operation_position = s.find('=');
  }
  if (operation_position == std::string_view::npos)
    throw std::invalid_argument{"Invalid lens string"};
  Lens lens{};
  lens.label = s.substr(0, operation_position);
  if (s[operation_position] == '=') {
    lens.focal_length = std::stoull(std::string{s.substr(operation_position + 1)});
  }
  return lens;
}

bool Lens::operator==(const Lens& other) const {
  return label == other.label;
}

std::size_t Lens::Hash(std::string_view s) {
  std::size_t hash{0};
  for (const auto c : s) {
    hash += static_cast<std::int32_t>(c);
    hash *= 17;
    hash %= 256;
  }
  return hash;
}

class Box {
 public:
  void AddReplaceLens(const Lens& lens);

  void RemoveLens(const Lens& lens);

  [[nodiscard]] const std::list<const Lens>& Lenses() const;

 private:
  std::list<const Lens> lenses_{};
};

void Box::AddReplaceLens(const Lens& lens) {
  if (auto it = std::ranges::find(lenses_, lens); it != lenses_.end()) {
    it = lenses_.erase(it);
    lenses_.insert(it, lens);
  } else {
    lenses_.push_back(lens);
  }
}

void Box::RemoveLens(const Lens& lens) {
  lenses_.remove(lens);
}

const std::list<const Lens>& Box::Lenses() const {
  return lenses_;
}

std::vector<Lens> ReadInitializationSequence(std::istream& is) {
  std::vector<Lens> initialization_sequence{};
  std::string line{};
  std::getline(is, line);
  std::istringstream iss{line};
  std::string segment{};
  while (std::getline(iss, segment, ',')) {
    initialization_sequence.push_back(Lens::Parse(segment));
  }
  return initialization_sequence;
}

int main() {
  const auto initialization_sequence = ReadInitializationSequence(std::cin);
  std::unordered_map<std::size_t, Box> boxes{};
  for (const auto& lens : initialization_sequence) {
    auto& box = boxes[lens.Box()];
    if (lens.focal_length.has_value()) {  // Equals
      box.AddReplaceLens(lens);
    } else {  // Dash
      box.RemoveLens(lens);
    }
  }
  std::size_t total_focusing_power{0};
  for (const auto& [_, box] : boxes) {
    std::size_t slot{1};
    for (const auto& lens : box.Lenses()) {
      total_focusing_power += lens.FocusingPower(slot++);
    }
  }
  std::cout << total_focusing_power << std::endl;
}
