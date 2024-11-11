import java.io.InputStream;
import java.util.*;

class Solution {

  private static List<Integer> ReadAdapterVoltages(final InputStream inputStream) {
    final Scanner scanner = new Scanner(inputStream);
    final List<Integer> voltages = new ArrayList<>();
    String line = scanner.nextLine();
    while (!line.isEmpty()) {
      voltages.add(Integer.valueOf(line));
      line = scanner.nextLine();
    }
    return voltages;
  }

  private static long DistinctArrangementCount(final List<Integer> sortedVoltages) {
    final long[] distinctUpTo = new long[sortedVoltages.getLast() + 1];
    distinctUpTo[0] = 1;
    for (final int v : sortedVoltages) {
      for (int d = 1; d <= 3; ++d) {
        if (v - d >= 0) {
          distinctUpTo[v] += distinctUpTo[v - d];
        }
      }
    }
    return distinctUpTo[sortedVoltages.getLast()];
  }

  public static void main(String[] args) {
    final List<Integer> voltages = ReadAdapterVoltages(System.in);
    voltages.add(voltages.stream().max(Integer::compareTo).get() + 3);
    voltages.sort(Integer::compareTo);
    final long distinctArrangementCount = DistinctArrangementCount(voltages);
    System.out.println(distinctArrangementCount);
  }
}