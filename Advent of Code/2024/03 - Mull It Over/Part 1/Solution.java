import java.util.Scanner;
import java.util.regex.MatchResult;
import java.util.regex.Pattern;

class Solution {

  private static long multiply(final String s) {
    final int a = Integer.parseInt(s.substring(s.indexOf('(') + 1, s.indexOf(',')));
    final int b = Integer.parseInt(s.substring(s.indexOf(',') + 1, s.indexOf(')')));
    return (long) a * b;
  }

  public static void main(final String[] args) {
    final Scanner scanner = new Scanner(System.in);
    final StringBuilder sb = new StringBuilder(scanner.nextLine());
    while (scanner.hasNextLine()) {
      final String line = scanner.nextLine();
      if (line.isEmpty()) {
        break;
      }
      sb.append(line);
    }
    final String s = sb.toString();
    final long multiplication = Pattern.compile("mul\\(\\d{1,3},\\d{1,3}\\)")
        .matcher(s)
        .results()
        .map(MatchResult::group)
        .map(Solution::multiply)
        .reduce(Long::sum)
        .orElseThrow();
    System.out.println(multiplication);
  }
}
