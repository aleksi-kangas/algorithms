import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

class Solution {

  private enum Position {
    FLOOR('.'), EMPTY('L'), OCCUPIED('#');

    private final char c;

    Position(final char c) {
      this.c = c;
    }

    public char C() {
      return c;
    }

    static Position Of(final char c) {
      return switch (c) {
        case '.' -> FLOOR;
        case 'L' -> EMPTY;
        case '#' -> OCCUPIED;
        default -> throw new IllegalArgumentException("Unexpected value: " + c);
      };
    }
  }

  private static final class SeatMap {

    private List<List<Position>> seatMap = new ArrayList<>();

    public SeatMap(final InputStream inputStream) {
      final Scanner scanner = new Scanner(inputStream);
      String line = scanner.nextLine();
      while (!line.isEmpty()) {
        final List<Position> row = new ArrayList<>(line.length());
        for (final char c : line.toCharArray()) {
          row.add(Position.Of(c));
        }
        seatMap.add(row);
        line = scanner.nextLine();
      }
    }

    public boolean Tick() {
      boolean anyChanges = false;
      final List<List<Position>> newSeatMap = deepCloneSeatMap();
      for (int r = 0; r < seatMap.size(); ++r) {
        for (int c = 0; c < seatMap.getFirst()
            .size(); ++c) {
          final Position position = seatMap.get(r)
              .get(c);
          switch (position) {
            case EMPTY -> {
              if (AdjacentOccupiedCount(r, c) == 0) {
                newSeatMap.get(r)
                    .set(c, Position.OCCUPIED);
                anyChanges = true;
              }
            }
            case OCCUPIED -> {
              if (AdjacentOccupiedCount(r, c) >= 4) {
                newSeatMap.get(r)
                    .set(c, Position.EMPTY);
                anyChanges = true;
              }
            }
          }
        }
      }
      seatMap = newSeatMap;
      return anyChanges;
    }

    public int OccupiedSeats() {
      int count = 0;
      for (final List<Position> row : seatMap) {
        for (final Position position : row) {
          if (position == Position.OCCUPIED) {
            ++count;
          }
        }
      }
      return count;
    }

    private int AdjacentOccupiedCount(final int r, final int c) {
      int count = 0;
      for (final int dr : new int[]{-1, 0, 1}) {
        for (final int dc : new int[]{-1, 0, 1}) {
          if (dr == 0 && dc == 0) {
            continue;
          }
          final int nr = r + dr;
          final int nc = c + dc;
          final boolean isValid =
              0 <= nr && nr < seatMap.size() && 0 <= nc && nc < seatMap.getFirst().size();
          if (isValid && seatMap.get(nr).get(nc) == Position.OCCUPIED) {
            ++count;
          }
        }
      }
      return count;
    }

    private List<List<Position>> deepCloneSeatMap() {
      final List<List<Position>> newSeatMap = new ArrayList<>(seatMap.size());
      for (final List<Position> row : seatMap) {
        final List<Position> newRow = new ArrayList<>(seatMap.size());
        newRow.addAll(row);
        newSeatMap.add(newRow);
      }
      return newSeatMap;
    }
  }

  public static void main(String[] args) {
    final SeatMap seatMap = new SeatMap(System.in);
    while (seatMap.Tick()) {
    }
    System.out.println(seatMap.OccupiedSeats());
  }
}
