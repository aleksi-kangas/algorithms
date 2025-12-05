import java.io.InputStream;
import java.util.*;

public final class Solution {

  private enum Cell {
    EMPTY('.'), PAPER('@');

    private final char c;

    Cell(final char c) {
      this.c = c;
    }

    public static Cell of(final char c) {
      return switch (c) {
        case '.' -> EMPTY;
        case '@' -> PAPER;
        default -> throw new IllegalArgumentException();
      };
    }

    @Override
    public String toString() {
      return String.valueOf(c);
    }
  }

  private final List<List<Cell>> grid = new ArrayList<>();

  public Solution(final InputStream inputStream) {
    final var scanner = new Scanner(inputStream);
    String line = scanner.nextLine();
    while (!line.isEmpty()) {
      final List<Cell> row = new ArrayList<>(line.length());
      for (final char c : line.toCharArray()) {
        row.add(Cell.of(c));
      }
      grid.add(row);
      line = scanner.nextLine();
    }
  }

  public int getAccessiblePaperCount() {
    int accessiblePaperCount = 0;
    for (int y = 0; y < grid.size(); ++y) {
      for (int x = 0; x < grid.getFirst().size(); ++x) {
        if (grid.get(y).get(x) == Cell.PAPER && getNeighboringCellCount(y, x, Cell.PAPER) < 4) {
          ++accessiblePaperCount;
        }
      }
    }
    return accessiblePaperCount;
  }

  private int getNeighboringCellCount(final int y, final int x, final Cell cellType) {
    int neighboringCellCount = 0;
    for (final int dx : new int[]{-1, 0, 1}) {
      for (final int dy : new int[]{-1, 0, 1}) {
        if ((dx == 0 && dy == 0) || (y + dy < 0 || grid.size() <= y + dy) || (x + dx < 0
            || grid.getFirst().size() <= x + dx)) {
          continue;
        }
        if (grid.get(y + dy).get(x + dx) == cellType) {
          ++neighboringCellCount;
        }
      }
    }
    return neighboringCellCount;
  }

  static void main(final String[] args) {
    final var solution = new Solution(System.in);
    System.out.println(solution.getAccessiblePaperCount());
  }
}