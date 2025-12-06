import java.io.InputStream;
import java.util.*;

public final class Solution {

  private final List<Range> ranges = new ArrayList<>();

  private record Range(long begin,
                       long end) {

    public long size() {
      return end - begin + 1;
    }

    public boolean overlaps(final Range other) {
      return (begin <= other.begin && other.begin <= end) || (begin <= other.end
          && other.end <= end);
    }

    public Range combine(final Range other) {
      return new Range(Math.min(begin, other.begin), Math.max(end, other.end));
    }
  }

  public Solution(final InputStream inputStream) {
    final var scanner = new Scanner(inputStream);
    String line = scanner.nextLine();
    while (!line.isEmpty()) {
      final String[] parts = line.split("-");
      ranges.add(new Range(Long.parseLong(parts[0]), Long.parseLong(parts[1])));
      boolean anyRangesCombined;
      do {
        anyRangesCombined = combineRanges();
      } while (anyRangesCombined);
      line = scanner.nextLine();
    }
  }

  public long freshIngredientCount() {
    return ranges.stream().map(Range::size).reduce(0L, Long::sum);
  }

  private boolean combineRanges() {
    for (final Range r1 : ranges) {
      for (final Range r2 : ranges) {
        if (r1 != r2 && r1.overlaps(r2)) {
          ranges.add(r1.combine(r2));
          ranges.remove(r1);
          ranges.remove(r2);
          return true;
        }
      }
    }
    return false;
  }

  static void main(final String[] args) {
    final var solution = new Solution(System.in);
    System.out.println(solution.freshIngredientCount());
  }
}