import java.io.InputStream;
import java.util.*;

class Solution {

  static final class Map {

    private final Set<Pair<Integer, Integer>> trees = new HashSet<>();
    private final Pair<Integer, Integer> size;

    Map(final InputStream inputStream) {
      final Scanner scanner = new Scanner(inputStream);
      String s = scanner.nextLine();
      int x = 0;
      int y = 0;
      while (!s.isEmpty()) {
        for (int i = 0; i < s.length(); ++i) {
          if (s.charAt(i) == '#') {
            trees.add(new Pair<>(i, y));
          }
        }
        x = Math.max(x, s.length());
        ++y;
        s = scanner.nextLine();
      }
      size = new Pair<>(x, y);
    }

    int CountTrees(final Pair<Integer, Integer> slope) {
      int count = 0;
      int x = 0;
      int y = 0;
      while (y <= size.v2) {
        if (trees.contains(new Pair<>(x, y))) {
          ++count;
        }
        x += slope.v1;
        x %= size.v1;
        y += slope.v2;
      }
      return count;
    }
  }

  public static void main(String[] args) {
    final Map map = new Map(System.in);
    final List<Pair<Integer, Integer>> slopes = List.of(new Pair<>(1, 1),
        new Pair<>(3, 1),
        new Pair<>(5, 1),
        new Pair<>(7, 1),
        new Pair<>(1, 2));
    final long solution = slopes.stream().mapToLong(map::CountTrees).reduce(1L, (a, b) -> a * b);
    System.out.println(solution);
  }

  private record Pair<T1, T2>(T1 v1, T2 v2) {

  }
}
