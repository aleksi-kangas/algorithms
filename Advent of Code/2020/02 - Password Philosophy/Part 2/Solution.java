import java.util.*;

class Solution {

  private record Policy(char character, Pair<Integer, Integer> positions) {

    boolean Pass(String password) {
      final boolean firstPositionContains = password.charAt(positions.v1 - 1) == character;
      final boolean secondPositionContains = password.charAt(positions.v2 - 1) == character;
      return firstPositionContains ^ secondPositionContains;
    }

    static Policy Parse(String line) {
      final int colonIndex = line.indexOf(":");
      final int dashIndex = line.indexOf("-");
      if (colonIndex == -1 || dashIndex == -1) {
        throw new IllegalArgumentException("Malformed line");
      }
      final int a = Integer.parseInt(line.substring(0, dashIndex));
      final int b = Integer.parseInt(line.substring(dashIndex + 1, colonIndex - 2));
      final char character = line.charAt(colonIndex - 1);
      return new Policy(character, new Pair<>(a, b));
    }
  }

  private static List<Pair<Policy, String>> ReadInput() {
    final Scanner scanner = new Scanner(System.in);
    final List<Pair<Policy, String>> input = new ArrayList<>();
    String s = scanner.nextLine();
    while (!s.isEmpty()) {
      final Policy policy = Policy.Parse(s);
      final String password = s.substring(s.indexOf(":") + 2);
      input.add(new Pair<>(policy, password));
      s = scanner.nextLine();
    }
    return input;
  }

  private static int Solve() {
    int count = 0;
    for (final Pair<Policy, String> p : ReadInput()) {
      if (p.v1.Pass(p.v2)) {
        ++count;
      }
    }
    return count;
  }

  public static void main(String[] args) {
    final int solution = Solve();
    System.out.println(solution);
  }

  private record Pair<T1, T2>(T1 v1, T2 v2) {

  }
}
