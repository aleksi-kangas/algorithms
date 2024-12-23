
import java.util.*;

public final class Solution {

  public static final class TopographicMap {

    public record Coordinates(int x, int y) {

    }

    private final List<List<Integer>> map = new ArrayList<>();

    public TopographicMap(final Scanner scanner) {
      String line = scanner.nextLine();
      while (!line.isEmpty()) {
        final List<Integer> row = new ArrayList<>(line.length());
        for (final char c : line.toCharArray()) {
          row.add(c - '0');
        }
        map.add(row);
        line = scanner.nextLine();
      }
    }

    public Set<Coordinates> trailHeads() {
      final Set<Coordinates> trailHeads = new HashSet<>();
      for (int y = 0; y < rows(); ++y) {
        for (int x = 0; x < columns(); ++x) {
          if (map.get(y).get(x) == 0) {
            trailHeads.add(new Coordinates(x, y));
          }
        }
      }
      return trailHeads;
    }

    public int trailHeadRating(final Coordinates trailHead) {
      final Set<List<Coordinates>> hikingTrails = new HashSet<>();
      final Set<Coordinates> visited = new HashSet<>();
      final List<Coordinates> path = new ArrayList<>();
      dfs(trailHead, visited, path, hikingTrails);
      return hikingTrails.size();
    }

    public int get(final Coordinates c) {
      return map.get(c.y).get(c.x);
    }

    private int rows() {
      return map.size();
    }

    private int columns() {
      return map.getFirst().size();
    }

    private List<Coordinates> neighbours(final Coordinates c) {
      final List<Coordinates> n = new ArrayList<>();
      final Coordinates up = new Coordinates(c.x, c.y - 1);
      final Coordinates down = new Coordinates(c.x, c.y + 1);
      final Coordinates left = new Coordinates(c.x - 1, c.y);
      final Coordinates right = new Coordinates(c.x + 1, c.y);
      if (isValid(up)) {
        n.add(up);
      }
      if (isValid(down)) {
        n.add(down);
      }
      if (isValid(left)) {
        n.add(left);
      }
      if (isValid(right)) {
        n.add(right);
      }
      return n;
    }

    private boolean isIncreasingByOne(final Coordinates c1, final Coordinates c2) {
      final int h1 = get(c1);
      final int h2 = get(c2);
      return h2 - h1 == 1;
    }

    private boolean isValid(final Coordinates c) {
      return 0 <= c.y && c.y < rows() && 0 <= c.x && c.x < columns();
    }

    private void dfs(final Coordinates c,
        final Set<Coordinates> visited,
        final List<Coordinates> path,
        final Set<List<Coordinates>> distinctPaths) {
      visited.add(c);
      path.add(c);
      if (get(c) == 9) {
        distinctPaths.add(new ArrayList<>(path));
      } else {
        for (final Coordinates n : neighbours(c)) {
          if (visited.contains(n)) {
            continue;
          }
          if (!isIncreasingByOne(c, n)) {
            continue;
          }
          dfs(n, visited, path, distinctPaths);
        }
      }
      visited.remove(c);
      path.remove(c);
    }
  }

  public static void main(final String[] args) {
    final TopographicMap topographicMap = new TopographicMap(new Scanner(System.in));
    final Set<TopographicMap.Coordinates> trailHeads = topographicMap.trailHeads();
    int sum = 0;
    for (final TopographicMap.Coordinates trailHead : trailHeads) {
      sum += topographicMap.trailHeadRating(trailHead);
    }
    System.out.println(sum);
  }
}