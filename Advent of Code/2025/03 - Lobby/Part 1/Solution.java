import java.io.InputStream;
import java.util.*;

public final class Solution {

  private int joltageSum = 0;

  private record GreedyMax(int value,
                           int index) {

  }

  public Solution(final InputStream inputStream) {
    final var scanner = new Scanner(inputStream);
    String line = scanner.nextLine();
    while (!line.isEmpty()) {
      final GreedyMax firstGreedyMax = searchGreedyMax(line, 0, line.length() - 1);
      final GreedyMax secondGreedyMax = searchGreedyMax(line, firstGreedyMax.index + 1,
          line.length());
      joltageSum += firstGreedyMax.value * 10 + secondGreedyMax.value;
      line = scanner.nextLine();
    }
  }

  private GreedyMax searchGreedyMax(final String s, final int beginInclusive,
      final int endExclusive) {
    int max = 0;
    int maxIndex = -1;
    for (int i = beginInclusive; i < endExclusive; ++i) {
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