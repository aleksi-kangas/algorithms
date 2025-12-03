import java.io.InputStream;
import java.util.*;
import java.util.stream.LongStream;

public final class Solution {

  private long invalidIdSum = 0;

  public record Range(long begin,
                      long end) {

    public long invalidIdSum() {
      return LongStream.range(begin, end + 1)
          .filter(this::isInvalid)
          .sum();
    }

    public static Range parse(final String s) {
      final String[] parts = s.split("-");
      return new Range(Long.parseLong(parts[0]), Long.parseLong(parts[1]));
    }

    private boolean isInvalid(final long x) {
      final String s = String.valueOf(x);
      if (s.length() < 2) {
        return false;
      }
      for (int i = 1; i < s.length(); ++i) {
        if (s.substring(0, i).equals(s.substring(i))) {
          return true;
        }
      }
      return false;
    }
  }

  public Solution(final InputStream inputStream) {
    final var scanner = new Scanner(inputStream);
    final String line = scanner.nextLine();
    for (final String rangeString : line.split(",")) {
      final var range = Range.parse(rangeString);
      invalidIdSum += range.invalidIdSum();
    }
  }

  static void main(final String[] args) {
    final var solution = new Solution(System.in);
    System.out.println(solution.invalidIdSum);
  }
}