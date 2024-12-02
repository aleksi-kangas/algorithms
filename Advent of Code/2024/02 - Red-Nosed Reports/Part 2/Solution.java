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

  private static boolean allIncreasing(final List<Integer> report, final boolean canRemove) {
    for (int i = 1; i < report.size(); ++i) {
      final int l = report.get(i - 1);
      final int r = report.get(i);
      if (l >= r || Math.abs(l - r) > 3) {
        if (canRemove) {
          // Try left...
          report.remove(i - 1);
          final boolean repairedByRemovingLeft = allIncreasing(report, false);
          report.add(i - 1, l);
          if (repairedByRemovingLeft) {
            return true;
          }
          // ... try right
          report.remove(i);
          final boolean repairedByRemovingRight = allIncreasing(report, false);
          report.add(i, r);
          return repairedByRemovingRight;
        } else {
          return false;
        }
      }
    }
    return true;
  }

  private static boolean allDecreasing(final List<Integer> report, final boolean canRemove) {
    for (int i = 1; i < report.size(); ++i) {
      final int l = report.get(i - 1);
      final int r = report.get(i);
      if (l <= r || Math.abs(l - r) > 3) {
        if (canRemove) {
          // Try left...
          report.remove(i - 1);
          final boolean repairedByRemovingLeft = allDecreasing(report, false);
          report.add(i - 1, l);
          if (repairedByRemovingLeft) {
            return true;
          }
          // ... try right
          report.remove(i);
          final boolean repairedByRemovingRight = allDecreasing(report, false);
          report.add(i, r);
          return repairedByRemovingRight;
        } else {
          return false;
        }
      }
    }
    return true;
  }

  private static boolean isSafe(final List<Integer> report) {
    return allIncreasing(report, true) || allDecreasing(report, true);
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
