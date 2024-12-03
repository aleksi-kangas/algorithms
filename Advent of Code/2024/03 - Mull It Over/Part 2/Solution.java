import java.util.Scanner;
import java.util.regex.MatchResult;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

class Solution {

  private static long multiply(final String s) {
    final int a = Integer.parseInt(s.substring(s.indexOf('(') + 1, s.indexOf(',')));
    final int b = Integer.parseInt(s.substring(s.indexOf(',') + 1, s.indexOf(')')));
    return (long) a * b;
  }

  public static void main(final String[] args) {
    final Scanner scanner = new Scanner(System.in);
    final StringBuilder sb = new StringBuilder(scanner.nextLine());
    while (scanner.hasNextLine()) {
      final String line = scanner.nextLine();
      if (line.isEmpty()) {
        break;
      }
      sb.append(line);
    }
    final String s = sb.toString();

    final Pattern doPattern = Pattern.compile("do\\(\\)");
    final Pattern dontPattern = Pattern.compile("don't\\(\\)");
    final Pattern multiplyPattern = Pattern.compile("mul\\(\\d{1,3},\\d{1,3}\\)");

    final Matcher doMatcher = doPattern.matcher(s);
    final Matcher dontMatcher = dontPattern.matcher(s);
    final Matcher multiplyMatcher = multiplyPattern.matcher(s);

    boolean enabled = true;
    int i = 0;
    long multiplication = 0;
    while (multiplyMatcher.find(i)) {
      final boolean doMatch = doMatcher.find(i);
      final boolean dontMatch = dontMatcher.find(i);
      if (doMatch && dontMatch) {
        // mul -> do -> dont
        if (multiplyMatcher.start() < doMatcher.start()
            && doMatcher.start() < dontMatcher.start()) {
          if (enabled) {
            multiplication += multiply(multiplyMatcher.group());
          }
          i = multiplyMatcher.end();
        }
        // mul -> dont -> do
        else if (multiplyMatcher.start() < dontMatcher.start()
            && dontMatcher.start() < doMatcher.start()) {
          if (enabled) {
            multiplication += multiply(multiplyMatcher.group());
          }
          i = multiplyMatcher.end();
        }
        // do -> dont -> mul
        else if (doMatcher.start() < dontMatcher.start()
            && dontMatcher.start() < multiplyMatcher.start()) {
          enabled = true;
          i = doMatcher.end();
        }
        // do -> mul -> dont
        else if (doMatcher.start() < multiplyMatcher.start()
            && multiplyMatcher.start() < dontMatcher.start()) {
          enabled = true;
          i = doMatcher.end();
        }
        // dont -> do -> mul
        else if (dontMatcher.start() < doMatcher.start()
            && doMatcher.start() < multiplyMatcher.start()) {
          enabled = true;
          i = dontMatcher.end();
        }
        // dont -> mul -> do
        else if (dontMatcher.start() < multiplyMatcher.start()
            && multiplyMatcher.start() < doMatcher.start()) {
          enabled = false;
          i = dontMatcher.end();
        }
      } else if (doMatch) {
        // do -> mul
        if (doMatcher.start() < multiplyMatcher.start()) {
          enabled = true;
          i = doMatcher.end();
        }
        // mul -> do
        else if (multiplyMatcher.start() < doMatcher.start()) {
          if (enabled) {
            multiplication += multiply(multiplyMatcher.group());
          }
          i = multiplyMatcher.end();
        }
      } else if (dontMatch) {
        // dont -> mul
        if (dontMatcher.start() < multiplyMatcher.start()) {
          enabled = false;
          i = dontMatcher.end();
        }
        // mul -> dont
        else if (multiplyMatcher.start() < dontMatcher.start()) {
          if (enabled) {
            multiplication += multiply(multiplyMatcher.group());
          }
          i = multiplyMatcher.end();
        }
      } else {
        if (enabled) {
          multiplication += multiply(multiplyMatcher.group());
        }
        i = multiplyMatcher.end();
      }
    }
    System.out.println(multiplication);
  }
}
