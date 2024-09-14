import java.io.InputStream;
import java.util.*;

class Solution {

  record BoardingPass(int row, int column) {

    static BoardingPass Parse(String bsp) {
      if (bsp.length() != (7 + 3)) {
        throw new IllegalArgumentException("Malformed BSP");
      }
      Pair<Integer, Integer> rowRange = new Pair<>(0, 127);
      for (int i = 0; i < 7; ++i) {
        switch (bsp.charAt(i)) {
          case 'F' -> rowRange = new Pair<>(rowRange.v1, rowRange.v2 - Span(rowRange) / 2);
          case 'B' -> rowRange = new Pair<>(rowRange.v1 + Span(rowRange) / 2, rowRange.v2);
          default -> throw new IllegalArgumentException("Malformed BSP");
        }
      }
      Pair<Integer, Integer> columnRange = new Pair<>(0, 7);
      for (int i = 0; i < 3; ++i) {
        switch (bsp.charAt(i + 7)) {
          case 'L' ->
              columnRange = new Pair<>(columnRange.v1, columnRange.v2 - Span(columnRange) / 2);
          case 'R' ->
              columnRange = new Pair<>(columnRange.v1 + Span(columnRange) / 2, columnRange.v2);
          default -> throw new IllegalArgumentException("Malformed BSP");
        }
      }
      if (!rowRange.v1.equals(rowRange.v2) || !columnRange.v1.equals(columnRange.v2)) {
        throw new IllegalStateException("Failure to parse BSP");
      }

      return new BoardingPass(rowRange.v1, columnRange.v1);
    }

    int SeatId() {
      return row * 8 + column;
    }
  }

  private static int Span(Pair<Integer, Integer> p) {
    return p.v2 - p.v1 + 1;
  }

  private static List<BoardingPass> ParseInput(final InputStream inputStream) {
    final List<BoardingPass> boardingPasses = new ArrayList<>();
    final Scanner scanner = new Scanner(inputStream);
    String s = scanner.nextLine();
    while (!s.isEmpty()) {
      boardingPasses.add(BoardingPass.Parse(s));
      s = scanner.nextLine();
    }
    return boardingPasses;
  }

  public static void main(String[] args) {
    final List<BoardingPass> boardingPasses = ParseInput(System.in);
    final int solution = boardingPasses.stream().mapToInt(BoardingPass::SeatId).max().orElseThrow();
    System.out.println(solution);
  }

  private record Pair<T1, T2>(T1 v1, T2 v2) {

  }
}
