import java.io.InputStream;
import java.util.*;

public final class Solution {

  private final List<Range> ranges = new ArrayList<>();
  private int freshCount = 0;

  private record Range(long begin, long end) {

  }

  public Solution(final InputStream inputStream) {
    final var scanner = new Scanner(inputStream);
    String line = scanner.nextLine();
    while (!line.isEmpty()) {
      final String[] parts = line.split("-");
      ranges.add(new Range(Long.parseLong(parts[0]), Long.parseLong(parts[1])));
      line = scanner.nextLine();
    }
    line = scanner.nextLine();
    while (!line.isEmpty()) {
      final long x = Long.parseLong(line);
      if (ranges.stream().anyMatch(r -> r.begin <= x && x <= r.end)) {
        ++freshCount;
      }
      line = scanner.nextLine();
    }
  }

  static void main(final String[] args) {
    final var solution = new Solution(System.in);
    System.out.println(solution.freshCount);
  }
}