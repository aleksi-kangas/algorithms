import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

class Solution {

  private record Pair<T1, T2>(T1 v1, T2 v2) {

  }

  private static List<Integer> parseBusLines(final String line) {
    final List<Integer> busLines = new ArrayList<>();
    for (final String busLine : line.split(",")) {
      if (busLine.equals("x")) {
        continue;
      }
      busLines.add(Integer.parseInt(busLine));
    }
    return busLines;
  }

  private static Pair<Integer, Integer> computeEarliestBus(final int earliestPossible,
      final List<Integer> busLines) {
    final int latest = earliestPossible + busLines.stream().max(Integer::compareTo).orElseThrow();
    for (int x = earliestPossible; x <= latest; ++x) {
      for (final int b : busLines) {
        if (x % b == 0) {
          return new Pair<>(b, x);
        }
      }
    }
    throw new RuntimeException("No solution");
  }

  public static void main(String[] args) {
    final Scanner scanner = new Scanner(System.in);
    final int earliestPossible = Integer.parseInt(scanner.nextLine());
    final List<Integer> busLines = parseBusLines(scanner.nextLine());
    final Pair<Integer, Integer> p = computeEarliestBus(earliestPossible, busLines);
    System.out.println((long) p.v1 * (p.v2 - earliestPossible));
  }
}
