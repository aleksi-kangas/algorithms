
import java.util.*;

public final class Solution {

  public record Stone(long value) {

    public enum BlinkOperation {
      ONE, SPLIT, MULTIPLY
    }

    public BlinkOperation blink() {
      if (value == 0) {
        return BlinkOperation.ONE;
      } else if (toString().length() % 2 == 0) {
        return BlinkOperation.SPLIT;
      }
      return BlinkOperation.MULTIPLY;
    }

    @Override
    public String toString() {
      return String.valueOf(value);
    }
  }

  public static void blinkAll(final LinkedList<Stone> stones) {
    final ListIterator<Stone> it = stones.listIterator();
    while (it.hasNext()) {
      final Stone stone = it.next();
      switch (stone.blink()) {
        case ONE -> it.set(new Stone(1));
        case SPLIT -> {
          final String stringValue = stone.toString();
          final Stone first = new Stone(
              Long.parseLong(stringValue.substring(0, stringValue.length() / 2)));
          final Stone second = new Stone(
              Long.parseLong(stringValue.substring(stringValue.length() / 2)));
          it.set(first);
          it.add(second);
        }
        case MULTIPLY -> it.set(new Stone(stone.value * 2024));
      }
    }
  }

  public static LinkedList<Stone> readStones(final String line) {
    final LinkedList<Stone> stones = new LinkedList<>();
    Arrays.stream(line.split(" ")).forEach(n -> stones.add(new Stone(Integer.parseInt(n))));
    return stones;
  }

  public static void main(final String[] args) {
    final LinkedList<Stone> stones = readStones(new Scanner(System.in).nextLine());
    for (int i = 1; i <= 25; ++i) {
      blinkAll(stones);
    }
    System.out.println(stones.size());
  }
}