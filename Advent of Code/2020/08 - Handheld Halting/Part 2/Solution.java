import java.util.*;

class Solution {

  static class Instruction {

    enum Operation {
      NOP, ACCUMULATE, JUMP
    }

    private Operation operation;
    private final int argument;

    void SwapNopJump() {
      switch (operation) {
        case NOP -> operation = Operation.JUMP;
        case JUMP -> operation = Operation.NOP;
      }
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

    private Instruction(final Operation operation, final int argument) {
      this.operation = operation;
      this.argument = argument;
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

  private static OptionalInt Execute(final List<Instruction> instructions) {
    int accumulator = 0;
    final Set<Integer> executedLines = new HashSet<>();
    int i = 0;
    while (!executedLines.contains(i) && i < instructions.size()) {
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
    return executedLines.contains(i) ? OptionalInt.empty() : OptionalInt.of(accumulator);
  }

  private static int RepairExecute(final List<Instruction> instructions) {
    int i = 0;
    while (true) {
      OptionalInt accumulator = Execute(instructions);
      if (accumulator.isPresent()) {
        return accumulator.getAsInt();
      }

      do {
        ++i;
      }
      while (i < instructions.size()
          && instructions.get(i).operation == Instruction.Operation.ACCUMULATE);
      final Instruction instruction = instructions.get(i);
      instruction.SwapNopJump();
      accumulator = Execute(instructions);
      if (accumulator.isPresent()) {
        return accumulator.getAsInt();
      }
      instruction.SwapNopJump();
    }
  }

  public static void main(String[] args) {
    final List<Instruction> instructions = ParseInstructions(new Scanner(System.in));
    final int accumulator = RepairExecute(instructions);
    System.out.println(accumulator);
  }
}
