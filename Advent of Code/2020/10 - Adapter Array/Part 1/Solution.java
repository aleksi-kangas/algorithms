import java.io.InputStream;
import java.util.*;

class Scratch {

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

  private static Map<Integer, Integer> VoltageDifferenceFrequencies(
      final List<Integer> sortedVoltages) {
    final Map<Integer, Integer> voltageDifferenceFrequencies = new HashMap<>();
    for (int i = 1; i < sortedVoltages.size(); ++i) {
      final int voltageDifference = sortedVoltages.get(i) - sortedVoltages.get(i - 1);
      voltageDifferenceFrequencies.put(voltageDifference,
          voltageDifferenceFrequencies.getOrDefault(voltageDifference, 0) + 1);
    }
    return voltageDifferenceFrequencies;
  }

  public static void main(String[] args) {
    final List<Integer> voltages = ReadAdapterVoltages(System.in);
    voltages.add(0);
    voltages.add(voltages.stream().max(Integer::compareTo).get() + 3);
    voltages.sort(Integer::compareTo);
    final Map<Integer, Integer> voltageDifferenceFrequencies = VoltageDifferenceFrequencies(
        voltages);
    System.out.println(voltageDifferenceFrequencies.get(1) * voltageDifferenceFrequencies.get(3));
  }
}