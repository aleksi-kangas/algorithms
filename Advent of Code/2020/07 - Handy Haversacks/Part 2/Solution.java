import java.util.*;

class Solution {

  record Graph(Map<String, Map<String, Integer>> edges) {

    int RequiredCount(final String begin) {
      final Map<String, Integer> badEdges = edges.getOrDefault(begin, Map.of());
      if (badEdges.isEmpty()) {
        return 1;
      }
      int count = 1;
      for (final Map.Entry<String, Integer> badEdge : badEdges.entrySet()) {
        count += badEdge.getValue() * RequiredCount(badEdge.getKey());
      }
      return count;
    }

    static Graph Parse(final Scanner scanner) {
      final Map<String, Map<String, Integer>> edges = new HashMap<>();
      String line = scanner.nextLine();
      while (!line.isEmpty()) {
        final Map.Entry<String, Map<String, Integer>> bagEdges = ParseEdges(line.replace(".", ""));
        edges.put(bagEdges.getKey(), bagEdges.getValue());
        line = scanner.nextLine();
      }
      return new Graph(edges);
    }

    private static Map.Entry<String, Map<String, Integer>> ParseEdges(final String line) {
      final String delimiter = " bags contain ";
      final String identifier = line.substring(0, line.indexOf(delimiter));
      final String[] rules = line.substring(line
              .indexOf(delimiter) + delimiter.length())
          .split(", ");
      final Map<String, Integer> to = new HashMap<>();
      for (final String rule : rules) {
        if (rule.equals("no other bags")) {
          continue;
        }
        final String ruleIdentifier = rule
            .substring(rule.indexOf(" ") + 1)
            .replace(" bags", "")
            .replace(" bag", "")
            .strip();
        final int ruleCount = Integer.parseInt(rule.substring(0, rule.indexOf(" ")));
        to.put(ruleIdentifier, ruleCount);
      }
      return new AbstractMap.SimpleImmutableEntry<>(identifier, to);
    }
  }

  public static void main(String[] args) {
    final Graph graph = Graph.Parse(new Scanner(System.in));
    final String begin = "shiny gold";
    final int count = graph.RequiredCount(begin) - 1;
    System.out.println(count);
  }
}
