import java.util.*;

class Solution {

  private static int similarity(final List<Integer> left, final Map<Integer, Integer> right) {
    int similarity = 0;
    for (final int l : left) {
      similarity += l * right.getOrDefault(l, 0);
    }
    return similarity;
  }

  public static void main(final String[] args) {
    final List<Integer> left = new ArrayList<>();
    final Map<Integer, Integer> right = new HashMap<>();
    final Scanner scanner = new Scanner(System.in);
    String line = scanner.nextLine();
    while (!line.isEmpty()) {
      final String[] p = line.split("\\s+");
      final int l = Integer.parseInt(p[0]);
      final int r = Integer.parseInt(p[1]);
      left.add(l);
      right.put(r, right.getOrDefault(r, 0) + 1);
      line = scanner.nextLine();
    }
    System.out.println(similarity(left, right));
  }
}
