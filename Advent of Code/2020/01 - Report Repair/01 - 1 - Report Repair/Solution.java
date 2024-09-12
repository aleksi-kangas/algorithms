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

    private static Pair<Integer, Integer> Solve(final List<Integer> integers) {
        final Set<Integer> remaining = new HashSet<>();
        for (final int x : integers) {
            if (remaining.contains(TARGET - x)) {
                return new Pair<>(x, TARGET - x);
            }
            remaining.add(x);
        }
        throw new RuntimeException("No solution");
    }

    public static void main(String[] args) {
        final Pair<Integer, Integer> solution = Solve(ReadInput());
        System.out.println((long) solution.v1 * solution.v2);
    }

    private record Pair<T1, T2>(T1 v1, T2 v2) {
    }
}
