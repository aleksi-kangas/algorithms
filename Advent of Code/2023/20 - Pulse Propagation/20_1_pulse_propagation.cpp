#include <iostream>
#include <queue>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

template <class... Ts>
struct Overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

struct Pulse {
  enum class Type { kLow, kHigh };

  Type type{};
  std::string source{};
  std::string destination{};
};

template <class C>
class Module {
 public:
  std::string name{};
  std::vector<std::string> destinations{};

  [[nodiscard]] std::vector<Pulse> Process(Pulse&& pulse);
  [[nodiscard]] static Module Parse(std::string_view s);

 private:
  Module() = default;
  friend C;

  [[nodiscard]] C& Underlying() { return static_cast<C&>(*this); }

  [[nodiscard]] const C& Underlying() const { return static_cast<const C&>(*this); }
};

template <class C>
std::vector<Pulse> Module<C>::Process(Pulse&& pulse) {
  return Underlying().Process(std::move(pulse));
}

template <class C>
Module<C> Module<C>::Parse(std::string_view s) {
  Module module{};
  const auto arrow_position = s.find("->");
  if (arrow_position == std::string_view::npos)
    throw std::invalid_argument{"Invalid module"};
  module.name = std::string{s.substr(0, arrow_position - 1)};
  std::istringstream iss{std::string{s.substr(arrow_position + 2)}};
  std::string segment{};
  while (std::getline(iss, segment, ',')) {
    module.destinations.push_back(segment.substr(1));
  }
  return module;
}

class FlipFlopModule : public Module<FlipFlopModule> {
 public:
  enum class State { kOff, kOn };

  [[nodiscard]] std::vector<Pulse> Process(Pulse&& pulse);
  [[nodiscard]] static FlipFlopModule Parse(std::string_view line);

 private:
  State state_{State::kOff};

  explicit FlipFlopModule(Module&& module) : Module{std::move(module)} {}
};

std::vector<Pulse> FlipFlopModule::Process(Pulse&& pulse) {
  switch (pulse.type) {
    case Pulse::Type::kLow: {
      std::vector<Pulse> pulses{};
      pulses.reserve(destinations.size());
      switch (state_) {
        case State::kOff: {
          state_ = State::kOn;
          for (const auto& destination : destinations) {
            pulses.emplace_back(Pulse{.type = Pulse::Type::kHigh, .source = name, .destination = destination});
          }
        } break;
        case State::kOn: {
          state_ = State::kOff;
          for (const auto& destination : destinations) {
            pulses.emplace_back(Pulse{.type = Pulse::Type::kLow, .source = name, .destination = destination});
          }
        } break;
        default:
          throw std::logic_error{"Unhandled flip-flop state"};
      }
      return pulses;
    }
    case Pulse::Type::kHigh:
      return {};
    default:
      throw std::logic_error{"Unhandled pulse type"};
  }
}

FlipFlopModule FlipFlopModule::Parse(std::string_view line) {
  if (line.starts_with('%')) {
    return FlipFlopModule{Module::Parse(line.substr(1))};
  }
  throw std::invalid_argument{"Invalid flip-flop module"};
}

class ConjunctionModule : public Module<ConjunctionModule> {
 public:
  [[nodiscard]] std::vector<Pulse> Process(Pulse&& pulse);
  [[nodiscard]] static ConjunctionModule Parse(std::string_view line);
  void InitializeInputs(const std::vector<std::string>& inputs);

 private:
  std::unordered_map<std::string, Pulse::Type> last_{};

  explicit ConjunctionModule(Module&& module) : Module{std::move(module)} {}
};

std::vector<Pulse> ConjunctionModule::Process(Pulse&& pulse) {
  last_[pulse.source] = pulse.type;
  const bool all_high = std::ranges::all_of(last_, [](const auto& pair) { return pair.second == Pulse::Type::kHigh; });
  std::vector<Pulse> pulses{};
  pulses.reserve(destinations.size());
  for (const auto& destination : destinations) {
    pulses.emplace_back(
        Pulse{.type = all_high ? Pulse::Type::kLow : Pulse::Type::kHigh, .source = name, .destination = destination});
  }
  return pulses;
}

ConjunctionModule ConjunctionModule::Parse(std::string_view line) {
  if (line.starts_with('&')) {
    return ConjunctionModule{Module::Parse(line.substr(1))};
  }
  throw std::invalid_argument{"Invalid conjunction module"};
}

void ConjunctionModule::InitializeInputs(const std::vector<std::string>& inputs) {
  last_.clear();
  for (const auto& input : inputs) {
    last_.emplace(input, Pulse::Type::kLow);
  }
}

class BroadcastModule : public Module<BroadcastModule> {
 public:
  [[nodiscard]] std::vector<Pulse> Process(Pulse&& pulse);
  [[nodiscard]] static BroadcastModule Parse(std::string_view line);

 private:
  explicit BroadcastModule(Module&& module) : Module{std::move(module)} {}
};

std::vector<Pulse> BroadcastModule::Process(Pulse&& pulse) {
  std::vector<Pulse> pulses{};
  pulses.reserve(destinations.size());
  for (const auto& destination : destinations) {
    pulses.emplace_back(Pulse{.type = pulse.type, .source = name, .destination = destination});
  }
  return pulses;
}

BroadcastModule BroadcastModule::Parse(std::string_view line) {
  if (line.starts_with("broadcaster")) {
    return BroadcastModule{Module::Parse(line)};
  }
  throw std::invalid_argument{"Invalid broadcast module"};
}

using module_t = std::variant<std::monostate, FlipFlopModule, ConjunctionModule, BroadcastModule>;

std::unordered_map<std::string, module_t> ParseModules(std::istream& is) {
  std::unordered_map<std::string, module_t> modules{};
  std::string line{};
  while (std::getline(is, line) && !line.empty()) {
    if (line.starts_with('%')) {
      auto module = FlipFlopModule::Parse(line);
      modules[module.name] = std::move(module);
    } else if (line.starts_with('&')) {
      auto module = ConjunctionModule::Parse(line);
      modules[module.name] = std::move(module);
    } else if (line.starts_with("broadcaster")) {
      auto module = BroadcastModule::Parse(line);
      modules[module.name] = std::move(module);
    } else {
      throw std::invalid_argument{"Invalid module"};
    }
  }
  return modules;
}

void InitializeConjuctionInputs(std::unordered_map<std::string, module_t>& modules) {
  std::unordered_set<std::string> conjunction_names{};
  std::ranges::for_each(modules, [&](const auto& pair) {
    // clang-format off
    std::visit(Overloaded{
      [&](const ConjunctionModule& conjunction_module) {conjunction_names.insert(conjunction_module.name);
      },
      [](const auto&) {}}, pair.second);
    // clang-format on
  });
  std::unordered_map<std::string, std::vector<std::string>> conjunction_inputs{};
  std::ranges::for_each(modules, [&](const auto& pair) {
    // clang-format off
    std::visit(Overloaded{
      [&](const FlipFlopModule& flip_flop_module) {
        std::ranges::for_each(flip_flop_module.destinations, [&](const auto& destination) {
          if (conjunction_names.contains(destination)) {
            conjunction_inputs[destination].push_back(flip_flop_module.name);
          }
        });
      },
      [&](const BroadcastModule& broadcast_module) {
        std::ranges::for_each(broadcast_module.destinations, [&](const auto& destination) {
          if (conjunction_names.contains(destination)) {
            conjunction_inputs[destination].push_back(broadcast_module.name);
          }
        });
      },
      [](const auto&) {}}, pair.second);
    // clang-format on
  });
  std::ranges::for_each(modules, [&](auto& pair) {
    // clang-format off
    std::visit(Overloaded{
      [&](ConjunctionModule& conjunction_module) {
        conjunction_module.InitializeInputs(conjunction_inputs[conjunction_module.name]);
      },
      [](auto&) {}}, pair.second);
    // clang-format on
  });
}

int main() {
  auto modules = ParseModules(std::cin);
  InitializeConjuctionInputs(modules);
  std::queue<Pulse> q{};
  std::size_t low_pulse_count{0};
  std::size_t high_pulse_count{0};
  for (std::size_t button{1}; button <= 1000; ++button) {
    q.push(Pulse{.type = Pulse::Type::kLow, .source = "button", .destination = "broadcaster"});
    while (!q.empty()) {
      auto pulse = q.front();
      q.pop();
      switch (pulse.type) {
        case Pulse::Type::kLow: {
          ++low_pulse_count;
        } break;
        case Pulse::Type::kHigh: {
          ++high_pulse_count;
        } break;
        default:
          throw std::logic_error{"Unhandled pulse type"};
      }
      const auto module_it = modules.find(pulse.destination);
      if (module_it == modules.end())
        continue;
      // clang-format off
      std::visit(Overloaded{
        [&](FlipFlopModule& flip_flop_module) {
          auto pulses = flip_flop_module.Process(std::move(pulse));
          for (auto& p : pulses) {
            q.push(std::move(p));
          }
        },
        [&](ConjunctionModule& conjunction_module) {
          auto pulses = conjunction_module.Process(std::move(pulse));
          for (auto& p : pulses) {
            q.push(std::move(p));
          }
        },
        [&](BroadcastModule& broadcast_module) {
          auto pulses = broadcast_module.Process(std::move(pulse));
          for (auto& p : pulses) {
            q.push(std::move(p));
          }
        },
        [](auto&) {}}, module_it->second);
      // clang-format on
    }
  }
  std::cout << low_pulse_count * high_pulse_count << std::endl;
}
