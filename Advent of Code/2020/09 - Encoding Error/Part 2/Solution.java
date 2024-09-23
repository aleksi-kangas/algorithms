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

  private static Pair<Integer, Integer> SubArraySum(final List<Long> integers, final long x) {
    final List<Long> prefixSum = new ArrayList<>(integers.size());
    for (int i = 0; i < integers.size(); ++i) {
      prefixSum.add(i == 0
          ? integers.get(i)
          : prefixSum.get(i - 1) + integers.get(i));
    }
    for (int l = 0; l < integers.size() - 1; ++l) {
      for (int r = l + 1; r < integers.size(); ++r) {
        final long sum = l == 0
            ? prefixSum.get(r)
            : prefixSum.get(r) - prefixSum.get(l - 1);
        if (sum == x) {
          return new Pair<>(l, r);
        }
      }
    }
    throw new RuntimeException("No solution");
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
        final long x = integers.get(i);
        final Pair<Integer, Integer> subArray = SubArraySum(integers, x);
        long minimum = Long.MAX_VALUE;
        long maximum = Long.MIN_VALUE;
        for (int j = subArray.v1; j <= subArray.v2; ++j) {
          minimum = Math.min(minimum, integers.get(j));
          maximum = Math.max(maximum, integers.get(j));
        }
        System.out.println(minimum + maximum);
        return;
      }
    }
    throw new RuntimeException("No solution");
  }

  private record Pair<T1, T2>(T1 v1, T2 v2) {

  }
}
