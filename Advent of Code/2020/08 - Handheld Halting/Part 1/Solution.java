import java.util.*;

class Solution {

  record Instruction(Operation operation, int argument) {

    enum Operation {
      NOP, ACCUMULATE, JUMP
    }

    static Instruction Parse(final String line) {
      final Operation operation = switch (line.substring(0, 3)) {
        case "nop" -> Operation.NOP;
        case "acc" -> Operation.ACCUMULATE;
        case "jmp" -> Operation.JUMP;
        default -> throw new IllegalArgumentException("Unknown operation: " + line.substring(0, 3));
      };
      final int argument = Integer.parseInt(line.substring(4));
      return new Instruction(operation, argument);
    }
  }

  private static List<Instruction> ParseInstructions(final Scanner scanner) {
    final List<Instruction> instructions = new ArrayList<>();
    String line = scanner.nextLine();
    while (!line.isEmpty()) {
      instructions.add(Instruction.Parse(line));
      line = scanner.nextLine();
    }
    return instructions;
  }

  private static int Execute(final List<Instruction> instructions) {
    int accumulator = 0;
    final Set<Integer> executedLines = new HashSet<>();
    int i = 0;
    while (!executedLines.contains(i)) {
      executedLines.add(i);
      final Instruction instruction = instructions.get(i);
      switch (instruction.operation) {
        case JUMP -> i += instruction.argument;
        case ACCUMULATE -> {
          accumulator += instruction.argument;
          ++i;
        }
        case NOP -> ++i;
      }
    }
    return accumulator;
  }

  public static void main(String[] args) {
    final List<Instruction> instructions = ParseInstructions(new Scanner(System.in));
    final int accumulator = Execute(instructions);
    System.out.println(accumulator);
  }
}
