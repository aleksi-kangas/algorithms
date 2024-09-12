import java.util.*;

class Solution {
    private static final int TARGET = 2020;

    private static List<Integer> ReadInput() {
        final Scanner scanner = new Scanner(System.in);
        final List<Integer> input = new ArrayList<>();
        String s = scanner.nextLine();
        while (!s.isEmpty()) {
            input.add(Integer.valueOf(s));
            s = scanner.nextLine();
        }
        return input;
    }

    private static List<Integer> Solve(final List<Integer> integers) {
        final Map<Integer, Pair<Integer, Integer>> remaining = new HashMap<>();
        for (final int x : integers) {
            for (final int y : integers) {
                remaining.put(x + y, new Pair<>(x, y));
            }
        }
        for (final int x : integers) {
            if (remaining.containsKey(TARGET - x)) {
                final Pair<Integer, Integer> p = remaining.get(TARGET - x);
                return List.of(x, p.v1, p.v2);
            }
        }
        throw new RuntimeException("No solution");
    }

    public static void main(String[] args) {
        final List<Integer> solution = Solve(ReadInput());
        System.out.println(solution.stream().mapToLong(Integer::longValue).reduce(1, (x, y) -> x * y));
    }

    private record Pair<T1, T2>(T1 v1, T2 v2) {
    }
}
