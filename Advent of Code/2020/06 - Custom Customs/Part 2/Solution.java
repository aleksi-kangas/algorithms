import java.io.InputStream;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

class Solution {

  record Group(Set<Character> yes) {

    static Optional<Group> Parse(final Scanner scanner) {
      String s = scanner.nextLine();
      if (s.isEmpty()) {
        return Optional.empty();
      }
      Set<Character> intersection = IntStream.rangeClosed('a', 'z')
          .mapToObj(i -> (char) i)
          .collect(Collectors.toSet());
      while (!s.isEmpty()) {
        final Set<Character> yes = new HashSet<>();
        for (final char c : s.toCharArray()) {
          yes.add(c);
        }
        intersection = intersection.stream().filter(yes::contains).collect(Collectors.toSet());
        s = scanner.nextLine();
      }
      return Optional.of(new Group(intersection));
    }

    int YesCount() {
      return yes.size();
    }
  }

  private static List<Group> ParseInput(final InputStream inputStream) {
    final List<Group> groups = new ArrayList<>();
    final Scanner scanner = new Scanner(inputStream);
    Optional<Group> group = Group.Parse(scanner);
    while (group.isPresent()) {
      groups.add(group.get());
      group = Group.Parse(scanner);
    }
    return groups;
  }

  public static void main(String[] args) {
    final List<Group> groups = ParseInput(System.in);
    final int sum = groups.stream().mapToInt(Group::YesCount).reduce(0, Integer::sum);
    System.out.println(sum);
  }
}
