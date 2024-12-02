import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

class Solution {

  private static List<Integer> parseReport(final String line) {
    final List<Integer> report = new ArrayList<>();
    for (final String l : line.split(" ")) {
      report.add(Integer.valueOf(l));
    }
    return report;
  }

  private static boolean allIncreasing(final List<Integer> report) {
    for (int i = 1; i < report.size(); ++i) {
      final int l = report.get(i - 1);
      final int r = report.get(i);
      if (l >= r || Math.abs(l - r) > 3) {
        return false;
      }
    }
    return true;
  }

  private static boolean allDecreasing(final List<Integer> report) {
    for (int i = 1; i < report.size(); ++i) {
      final int l = report.get(i - 1);
      final int r = report.get(i);
      if (l <= r || Math.abs(l - r) > 3) {
        return false;
      }
    }
    return true;
  }

  private static boolean isSafe(final List<Integer> report) {
    return allIncreasing(report) || allDecreasing(report);
  }

  public static void main(final String[] args) {
    final Scanner scanner = new Scanner(System.in);
    String line = scanner.nextLine();
    int safeCount = 0;
    while (!line.isEmpty()) {
      final List<Integer> report = parseReport(line);
      if (isSafe(report)) {
        ++safeCount;
      }
      line = scanner.nextLine();
    }
    System.out.println(safeCount);
  }
}
