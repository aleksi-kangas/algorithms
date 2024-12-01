import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

class Solution {

  public static void main(final String[] args) {
    final List<Integer> left = new ArrayList<>();
    final List<Integer> right = new ArrayList<>();
    final Scanner scanner = new Scanner(System.in);
    String line = scanner.nextLine();
    while (!line.isEmpty()) {
      final String[] p = line.split("\\s+");
      left.add(Integer.valueOf(p[0]));
      right.add(Integer.valueOf(p[1]));
      line = scanner.nextLine();
    }
    left.sort(Integer::compareTo);
    right.sort(Integer::compareTo);
    int distanceSum = 0;
    for (int i = 0; i < left.size(); ++i) {
      distanceSum += Math.abs(left.get(i) - right.get(i));
    }
    System.out.println(distanceSum);
  }
}
