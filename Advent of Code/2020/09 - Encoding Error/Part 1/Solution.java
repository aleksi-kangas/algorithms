import java.util.*;

class Solution {

  private static final Integer PREAMBLE = 25;

  private static boolean IsSumOfAnyTwoInRange(final List<Long> integers, final int l, final int r,
      final int i) {
    final Set<Long> s = new HashSet<>();
    for (int j = l; j <= r; ++j) {
      if (s.contains(integers.get(j))) {
        return true;
      }
      s.add(integers.get(i) - integers.get(j));
    }
    return false;
  }

  private static List<Long> ParseInput(final Scanner scanner) {
    final List<Long> integers = new ArrayList<>();
    String s = scanner.nextLine();
    while (!s.isEmpty()) {
      integers.add(Long.parseLong(s));
      s = scanner.nextLine();
    }
    return integers;
  }

  public static void main(String[] args) {
    final List<Long> integers = ParseInput(new Scanner(System.in));
    for (int i = PREAMBLE; i < integers.size(); ++i) {
      final int l = i - PREAMBLE;
      final int r = i - 1;
      if (!IsSumOfAnyTwoInRange(integers, l, r, i)) {
        System.out.println(integers.get(i));
        return;
      }
    }
    throw new RuntimeException("No solution");
  }
}
