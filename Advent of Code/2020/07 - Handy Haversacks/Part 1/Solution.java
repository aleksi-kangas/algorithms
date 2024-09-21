import java.util.*;

class Solution {

  record Graph(Map<String, Set<String>> edges) {

    boolean DFS(final String begin, final String target) {
      if (begin.equals(target)) {
        return true;
      }
      final Set<String> visited = new HashSet<>();
      final Stack<String> s = new Stack<>();
      visited.add(begin);
      s.add(begin);
      while (!s.isEmpty()) {
        final String b = s.pop();
        if (b.equals(target)) {
          return true;
        }
        visited.add(b);
        s.addAll(edges.getOrDefault(b, Set.of()));
      }
      return false;
    }

    static Graph Parse(final Scanner scanner) {
      final Map<String, Set<String>> edges = new HashMap<>();
      String line = scanner.nextLine();
      while (!line.isEmpty()) {
        final Map.Entry<String, Set<String>> bagEdges = ParseEdges(line.replace(".", ""));
        edges.put(bagEdges.getKey(), bagEdges.getValue());
        line = scanner.nextLine();
      }
      return new Graph(edges);
    }

    private static Map.Entry<String, Set<String>> ParseEdges(final String line) {
      final String delimiter = " bags contain ";
      final String identifier = line.substring(0, line.indexOf(delimiter));
      final String[] rules = line.substring(line
              .indexOf(delimiter) + delimiter.length())
          .split(", ");
      final Set<String> to = new HashSet<>();
      for (final String rule : rules) {
        final String ruleIdentifier = rule
            .substring(rule.indexOf(" ") + 1)
            .replace(" bags", "")
            .replace(" bag", "")
            .strip();
        to.add(ruleIdentifier);
      }
      return new AbstractMap.SimpleImmutableEntry<>(identifier, to);
    }
  }

  public static void main(String[] args) {
    final Graph graph = Graph.Parse(new Scanner(System.in));
    final String target = "shiny gold";
    final int count = graph.edges.keySet()
        .stream()
        .filter(b -> !b.equals(target))
        .mapToInt(b -> graph.DFS(b, target) ? 1 : 0)
        .reduce(Integer::sum)
        .orElseThrow();
    System.out.println(count);
  }
}
