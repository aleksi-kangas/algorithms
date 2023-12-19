#include <iostream>
#include <istream>
#include <optional>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct Part {
  std::size_t x{};
  std::size_t m{};
  std::size_t a{};
  std::size_t s{};

  [[nodiscard]] static Part Parse(std::string_view line);
};

Part Part::Parse(std::string_view line) {
  line = line.substr(1, line.size() - 2);
  const auto x_position = line.find("x=");
  const auto m_position = line.find("m=", x_position);
  const auto a_position = line.find("a=", m_position);
  const auto s_position = line.find("s=", a_position);
  if (x_position == std::string_view::npos || m_position == std::string_view::npos ||
      a_position == std::string_view::npos || s_position == std::string_view::npos)
    throw std::invalid_argument{"Invalid part"};
  return {.x = std::stoul(std::string{line.substr(x_position + 2, m_position - x_position - 3)}),
          .m = std::stoul(std::string{line.substr(m_position + 2, a_position - m_position - 3)}),
          .a = std::stoul(std::string{line.substr(a_position + 2, s_position - a_position - 3)}),
          .s = std::stoul(std::string{line.substr(s_position + 2)})};
}

struct Rule {
  enum class Type { kX, kM, kA, kS };
  enum class Operation { KGreaterThan, kLessThan };

  std::optional<Type> type{std::nullopt};
  std::optional<Operation> operation{std::nullopt};
  std::optional<std::size_t> threshold{std::nullopt};
  std::string target{};

  [[nodiscard]] std::optional<std::string> Apply(const Part& part) const;

  [[nodiscard]] static Rule Parse(std::string_view s);
};

std::optional<std::string> Rule::Apply(const Part& part) const {
  if (!type.has_value() || !operation.has_value() || !threshold.has_value())
    return target;
  switch (operation.value()) {
    case Operation::kLessThan: {
      switch (type.value()) {
        case Type::kX:
          return part.x < threshold.value() ? std::make_optional(target) : std::nullopt;
        case Type::kM:
          return part.m < threshold.value() ? std::make_optional(target) : std::nullopt;
        case Type::kA:
          return part.a < threshold.value() ? std::make_optional(target) : std::nullopt;
        case Type::kS:
          return part.s < threshold.value() ? std::make_optional(target) : std::nullopt;
        default:
          throw std::logic_error{"Unhandled type"};
      }
    } break;
    case Operation::KGreaterThan: {
      switch (type.value()) {
        case Type::kX:
          return part.x > threshold.value() ? std::make_optional(target) : std::nullopt;
        case Type::kM:
          return part.m > threshold.value() ? std::make_optional(target) : std::nullopt;
        case Type::kA:
          return part.a > threshold.value() ? std::make_optional(target) : std::nullopt;
        case Type::kS:
          return part.s > threshold.value() ? std::make_optional(target) : std::nullopt;
        default:
          throw std::logic_error{"Unhandled type"};
      }
    } break;
    default:
      throw std::logic_error{"Unhandled operation"};
  }
}

Rule Rule::Parse(std::string_view s) {
  Rule rule{};
  if (s.find(':') == std::string_view::npos) {
    rule.target = std::string{s};
    return rule;
  }
  switch (s[0]) {
    case 'x': {
      rule.type = Type::kX;
    } break;
    case 'm': {
      rule.type = Type::kM;
    } break;
    case 'a': {
      rule.type = Type::kA;
    } break;
    case 's': {
      rule.type = Type::kS;
    } break;
    default:
      throw std::invalid_argument{"Invalid rule"};
  }
  switch (s[1]) {
    case '<': {
      rule.operation = Operation::kLessThan;
    } break;
    case '>': {
      rule.operation = Operation::KGreaterThan;
    } break;
    default:
      throw std::invalid_argument{"Invalid rule"};
  }
  const auto colon_position = s.find(':');
  if (colon_position == std::string_view::npos)
    throw std::invalid_argument{"Invalid rule"};
  rule.threshold = std::stoul(std::string{s.substr(2, colon_position - 2)});
  rule.target = std::string{s.substr(colon_position + 1)};
  return rule;
}

struct Workflow {
  std::string name{};
  std::vector<Rule> rules{};

  [[nodiscard]] static Workflow Parse(std::string_view line);
};

Workflow Workflow::Parse(std::string_view line) {
  Workflow workflow{};
  const auto brace_position = line.find('{');
  if (brace_position == std::string_view::npos)
    throw std::invalid_argument{"Invalid workflow"};
  workflow.name = std::string{line.substr(0, brace_position)};
  std::istringstream iss{std::string{line.substr(brace_position + 1, line.size() - brace_position - 2)}};
  std::string segment{};
  while (std::getline(iss, segment, ',')) {
    workflow.rules.push_back(Rule::Parse(segment));
  }
  return workflow;
}

int main() {
  std::unordered_map<std::string, Workflow> workflows{};
  std::string line{};
  while (std::getline(std::cin, line) && !line.empty()) {
    auto workflow = Workflow::Parse(line);
    workflows[workflow.name] = std::move(workflow);
  }
  std::queue<Part> parts{};
  while (std::getline(std::cin, line) && !line.empty()) {
    parts.push(Part::Parse(line));
  }
  std::size_t total{0};
  std::string workflow_name{"in"};
  std::optional<Part> part{std::nullopt};
  while (!parts.empty() || part.has_value()) {
    if (!part.has_value()) {
      part = parts.front();
      parts.pop();
    }
    const auto& workflow = workflows[workflow_name];
    for (const auto& rule : workflow.rules) {
      const auto result = rule.Apply(part.value());
      if (!result.has_value())
        continue;
      workflow_name = result.value() == "A" || result.value() == "R" ? "in" : result.value();
      if (result.value() == "A") {
        total += part->x;
        total += part->m;
        total += part->a;
        total += part->s;
      }
      if (result.value() == "A" || result.value() == "R") {
        part.reset();
      }
      break;
    }
  }
  std::cout << total << std::endl;
}
