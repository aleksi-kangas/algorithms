import java.io.InputStream;
import java.util.*;

public final class Solution {

  private long joltageSum = 0;

  private record GreedyMax(int value,
                           int index) {

  }

  public Solution(final InputStream inputStream) {
    final var scanner = new Scanner(inputStream);
    String line = scanner.nextLine();
    while (!line.isEmpty()) {
      int greedySearchBegin = 0;
      for (int digit = 12; digit >= 1; --digit) {
        final GreedyMax greedyMax = searchGreedyMax(line, greedySearchBegin, line.length() - digit);
        joltageSum += Math.powExact(10L, digit - 1) * greedyMax.value;
        greedySearchBegin = greedyMax.index + 1;
      }
      line = scanner.nextLine();
    }
  }

  private GreedyMax searchGreedyMax(final String s, final int beginInclusive,
      final int endInclusive) {
    int max = 0;
    int maxIndex = -1;
    for (int i = beginInclusive; i <= endInclusive; ++i) {
      final int x = s.charAt(i) - '0';
      if (x > max) {
        max = x;
        maxIndex = i;
      }
    }
    return new GreedyMax(max, maxIndex);
  }

  static void main(final String[] args) {
    final var solution = new Solution(System.in);
    System.out.println(solution.joltageSum);
  }
}