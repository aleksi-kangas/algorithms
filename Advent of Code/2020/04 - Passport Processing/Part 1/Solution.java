import java.io.InputStream;
import java.util.*;

class Solution {

  record Passport(Map<String, String> fields) {

    private enum REQUIRED_FIELD {
      BIRTH_YEAR("byr"),
      ISSUE_YEAR("iyr"),
      EXPIRATION_YEAR("eyr"),
      HEIGHT("hgt"),
      HAIR_COLOR("hcl"),
      EYE_COLOR("ecl"),
      PASSPORT_ID("pid");

      final String field;

      REQUIRED_FIELD(final String field) {
        this.field = field;
      }
    }

    private enum OPTIONAL_FIELD {
      COUNTRY_ID("cid");

      final String field;

      OPTIONAL_FIELD(final String field) {
        this.field = field;
      }
    }

    boolean IsValid() {
      return Arrays.stream(REQUIRED_FIELD.values())
          .allMatch(requiredField -> fields.containsKey(requiredField.field));
    }

    static Optional<Passport> Parse(final Scanner scanner) {
      String s = scanner.nextLine();
      if (s.isEmpty()) {
        return Optional.empty();
      }
      final Map<String, String> m = new HashMap<>();
      while (!s.isEmpty()) {
        m.putAll(ParseLine(s));
        s = scanner.nextLine();
      }
      return Optional.of(new Passport(m));
    }

    private static Map<String, String> ParseLine(final String line) {
      final Map<String, String> m = new HashMap<>();
      for (final String part : line.split(" ")) {
        final String[] values = part.split(":");
        if (values.length != 2) {
          throw new IllegalArgumentException("Malformed input");
        }
        m.put(values[0], values[1]);
      }
      return m;
    }
  }

  private static List<Passport> ParseInput(final InputStream inputStream) {
    final List<Passport> passports = new ArrayList<>();
    final Scanner scanner = new Scanner(inputStream);
    Optional<Passport> passport = Passport.Parse(scanner);
    while (passport.isPresent()) {
      passports.add(passport.get());
      passport = Passport.Parse(scanner);
    }
    return passports;
  }

  public static void main(String[] args) {
    final List<Passport> passports = ParseInput(System.in);
    final int solution = passports.stream().mapToInt(p -> p.IsValid() ? 1 : 0)
        .reduce(0, Integer::sum);
    System.out.println(solution);
  }
}
