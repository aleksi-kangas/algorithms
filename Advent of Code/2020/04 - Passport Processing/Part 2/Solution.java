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
      return Arrays.stream(REQUIRED_FIELD.values()).allMatch(this::IsRequiredFieldValid);
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

    private boolean IsRequiredFieldValid(final REQUIRED_FIELD requiredField) {
      try {
        if (!fields.containsKey(requiredField.field)) {
          return false;
        }
        return switch (requiredField) {
          case BIRTH_YEAR -> IsBirthYearValid();
          case ISSUE_YEAR -> IsIssueYearValid();
          case EXPIRATION_YEAR -> IsExpirationYearValid();
          case HEIGHT -> IsHeightValid();
          case HAIR_COLOR -> IsHairColorValid();
          case EYE_COLOR -> IsEyeColorValid();
          case PASSPORT_ID -> IsPassportIdValid();
          default ->
              throw new IllegalArgumentException("Unknown required field: " + requiredField.field);
        };
      } catch (final RuntimeException e) {
        return false;
      }
    }

    private boolean IsBirthYearValid() throws NumberFormatException {
      final int birthYear = Integer.parseInt(fields.get(REQUIRED_FIELD.BIRTH_YEAR.field));
      return 1920 <= birthYear && birthYear <= 2002;
    }

    private boolean IsIssueYearValid() throws NumberFormatException {
      final int issueYear = Integer.parseInt(fields.get(REQUIRED_FIELD.ISSUE_YEAR.field));
      return 2010 <= issueYear && issueYear <= 2020;
    }

    private boolean IsExpirationYearValid() throws NumberFormatException {
      final int expirationYear = Integer.parseInt(fields.get(REQUIRED_FIELD.EXPIRATION_YEAR.field));
      return 2020 <= expirationYear && expirationYear <= 2030;
    }

    private boolean IsHeightValid() throws IllegalArgumentException {
      final String value = fields.get(REQUIRED_FIELD.HEIGHT.field);
      final int height = Integer.parseInt(value.substring(0, value.length() - 2));
      final String unit = value.substring(value.length() - 2);
      return switch (unit) {
        case "cm" -> 150 <= height && height <= 193;
        case "in" -> 59 <= height && height <= 76;
        default -> throw new IllegalArgumentException("Unknown height unit: " + unit);
      };
    }

    private boolean IsHairColorValid() throws NumberFormatException {
      final String hairColor = fields.get(REQUIRED_FIELD.HAIR_COLOR.field);
      if (hairColor.length() != 7) {
        return false;
      }
      if (!hairColor.startsWith("#")) {
        return false;
      }
      //noinspection ResultOfMethodCallIgnored
      Integer.parseInt(hairColor.substring(1), 16);
      return true;
    }

    private boolean IsEyeColorValid() {
      final String eyeColor = fields.get(REQUIRED_FIELD.EYE_COLOR.field);
      return List.of("amb", "blu", "brn", "gry", "grn", "hzl", "oth").contains(eyeColor);
    }

    private boolean IsPassportIdValid() {
      final String passportId = fields.get(REQUIRED_FIELD.PASSPORT_ID.field);
      return passportId.length() == 9 && passportId.chars().allMatch(Character::isDigit);
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
