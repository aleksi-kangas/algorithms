import java.io.InputStream;
import java.util.*;
import java.util.function.Predicate;

public final class Solution {

  private final List<Operation> operations = new ArrayList<>();

  public static final class Operation {

    public enum Type {
      ADDITION, MULTIPLICATION;

      public static Type of(final char c) {
        return switch (c) {
          case '+' -> ADDITION;
          case '*' -> MULTIPLICATION;
          default -> throw new IllegalArgumentException();
        };
      }
    }

    private final List<Integer> numbers = new ArrayList<>();
    private Type type = null;

    public void add(final int number) {
      this.numbers.add(number);
    }

    public void setType(final Type type) {
      this.type = Objects.requireNonNull(type);
    }

    public long compute() {
      return switch (type) {
        case ADDITION -> numbers.stream().map(Long::valueOf).reduce(0L, Long::sum);
        case MULTIPLICATION -> numbers.stream().map(Long::valueOf).reduce(1L, Math::multiplyExact);
      };
    }
  }

  public Solution(final InputStream inputStream) {
    final var scanner = new Scanner(inputStream);
    String line = scanner.nextLine().strip();
    while (!line.isEmpty()) {
      final char firstChar = line.charAt(0);
      if (Character.isDigit(firstChar)) {
        final List<Integer> numbers = Arrays.stream(line.split(" ")).map(String::strip)
            .filter(Predicate.not(String::isEmpty)).map(Integer::parseInt).toList();
        if (operations.isEmpty()) {
          for (final int x : numbers) {
            final var operation = new Operation();
            operation.add(x);
            operations.add(operation);
          }
        } else {
          if (numbers.size() != operations.size()) {
            throw new IllegalArgumentException();
          }
          for (int i = 0; i < numbers.size(); ++i) {
            operations.get(i).add(numbers.get(i));
          }
        }

      } else if (firstChar == '+' || firstChar == '*') {
        final List<Operation.Type> types = Arrays.stream(line.split(" ")).map(String::strip)
            .filter(Predicate.not(String::isEmpty)).map(s -> s.charAt(0)).map(Operation.Type::of)
            .toList();
        if (types.size() != operations.size()) {
          throw new IllegalArgumentException();
        }
        for (int i = 0; i < types.size(); ++i) {
          operations.get(i).setType(types.get(i));
        }
      } else {
        throw new IllegalArgumentException();
      }
      line = scanner.nextLine().strip();
    }
  }

  public long grandTotal() {
    return operations.stream().map(Operation::compute).reduce(0L, Long::sum);
  }

  static void main(final String[] args) {
    final var solution = new Solution(System.in);
    System.out.println(solution.grandTotal());
  }
}