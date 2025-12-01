import java.io.InputStream;
import java.util.*;
import java.util.stream.IntStream;

public final class Solution {

  private int dial = 50;
  private int zeroCounter = 0;

  public enum Direction {
    LEFT("L"),
    RIGHT("R");
    private final String value;

    Direction(final String value) {
      this.value = value;
    }

    public static Direction from(final String s) {
      return Arrays.stream(values())
          .filter(direction -> direction.value.equals(s))
          .findFirst()
          .orElseThrow();
    }
  }

  public Solution(final InputStream inputStream) {
    final var scanner = new Scanner(inputStream);
    String line = scanner.nextLine();
    while (!line.isEmpty()) {
      final var direction = Direction.from(line.substring(0, 1));
      final int value = Integer.parseInt(line.substring(1));
      switch (direction) {
        case LEFT -> IntStream.range(0, value).forEach(_ -> {
          --dial;
          dial %= 100;
          if (dial == 0) {
            ++zeroCounter;
          }
        });
        case RIGHT -> IntStream.range(0, value).forEach(_ -> {
          ++dial;
          dial %= 100;
          if (dial == 0) {
            ++zeroCounter;
          }
        });
      }
      line = scanner.nextLine();
    }
  }

  public int getZeroCounter() {
    return zeroCounter;
  }

  static void main(final String[] args) {
    final var solution = new Solution(System.in);
    System.out.println(solution.getZeroCounter());
  }
}