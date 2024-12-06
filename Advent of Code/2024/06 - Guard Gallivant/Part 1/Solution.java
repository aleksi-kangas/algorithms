import java.util.*;

class Solution {

  private record Coordinates(int x, int y) {

  }

  private static final class Guard {

    enum Direction {NORTH, EAST, SOUTH, WEST;}

    private final Set<Coordinates> visited = new HashSet<>();

    private Direction direction;
    private Coordinates c;

    public Guard(final Coordinates coordinates, final Direction direction) {
      this.c = coordinates;
      this.direction = direction;
      visited.add(c);
    }

    public Coordinates getCoordinates() {
      return c;
    }

    public void turnRight() {
      direction = switch (direction) {
        case NORTH -> Direction.EAST;
        case EAST -> Direction.SOUTH;
        case SOUTH -> Direction.WEST;
        case WEST -> Direction.NORTH;
      };
    }

    public boolean move(final LabMap labMap) {
      final Coordinates next = switch (direction) {
        case NORTH -> new Coordinates(c.x, c.y - 1);
        case EAST -> new Coordinates(c.x + 1, c.y);
        case SOUTH -> new Coordinates(c.x, c.y + 1);
        case WEST -> new Coordinates(c.x - 1, c.y);
      };
      final LabMap.Tile tile = labMap.get(next);
      return switch (tile) {
        case UNKNOWN -> true;
        case EMPTY -> {
          this.c = next;
          visited.add(next);
          yield false;
        }
        case OBSTACLE -> {
          turnRight();
          yield false;
        }
      };
    }

    public int getVisitedCount() {
      return visited.size();
    }
  }

  private static final class LabMap {

    enum Tile {UNKNOWN, EMPTY, OBSTACLE}

    private final List<List<Tile>> m = new ArrayList<>();
    private Guard guard;

    public LabMap(final Scanner scanner) {
      String line = scanner.nextLine();
      int y = 0;
      while (!line.isEmpty()) {
        final List<Tile> r = new ArrayList<>();
        int x = 0;
        for (final char c : line.toCharArray()) {
          final Coordinates coordinates = new Coordinates(x, y);
          switch (c) {
            case '.' -> r.add(Tile.EMPTY);
            case '#' -> r.add(Tile.OBSTACLE);
            case '^' -> {
              r.add(Tile.EMPTY);
              guard = new Guard(coordinates, Guard.Direction.NORTH);
            }
            case '>' -> {
              r.add(Tile.EMPTY);
              guard = new Guard(coordinates, Guard.Direction.EAST);
            }
            case '<' -> {
              r.add(Tile.EMPTY);
              guard = new Guard(coordinates, Guard.Direction.WEST);
            }
            case 'v' -> {
              r.add(Tile.EMPTY);
              guard = new Guard(coordinates, Guard.Direction.SOUTH);
            }
          }
          ++x;
        }
        m.add(r);
        ++y;
        line = scanner.nextLine();
      }
    }

    public Tile get(final Coordinates c) {
      if (0 <= c.y && c.y < m.size() && 0 <= c.x && c.x < m.getFirst()
          .size()) {
        return m.get(c.y)
            .get(c.x);
      }
      return Tile.UNKNOWN;
    }

    public Guard getGuard() {
      return guard;
    }

    public void simulate() {
      while (!guard.move(this)) {
      }
    }

    public void print() {
      for (int y = 0; y < m.size(); ++y) {
        final List<Tile> r = m.get(y);
        for (int x = 0; x < r.size(); ++x) {
          final Tile t = r.get(x);
          if (guard.getCoordinates().y == y && guard.getCoordinates().x == x) {
            System.out.print("G");
          } else {
            switch (t) {
              case EMPTY -> System.out.print(".");
              case OBSTACLE -> System.out.print("#");
            }
          }
        }
        System.out.println();
      }
    }
  }

  public static void main(final String[] args) {
    final Scanner scanner = new Scanner(System.in);
    final LabMap labMap = new LabMap(scanner);
    labMap.simulate();
    System.out.println(labMap.getGuard()
        .getVisitedCount());
  }
}
