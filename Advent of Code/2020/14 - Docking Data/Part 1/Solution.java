import java.util.BitSet;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

class Solution {

  private static final int SIZE = 36;

  private static final class BitMask {

    private final BitSet zeroBitSet = new BitSet(SIZE);
    private final BitSet oneBitSet = new BitSet(SIZE);

    public BitMask(final String line) {
      for (int i = 0; i < line.length(); ++i) {
        final char c = line.charAt(i);
        final int j = line.length() - i - 1;
        switch (c) {
          case '0' -> zeroBitSet.set(j);
          case '1' -> oneBitSet.set(j);
        }
      }
    }

    public void apply(final BitSet bitSet) {
      applyMask(bitSet, zeroBitSet, false);
      applyMask(bitSet, oneBitSet, true);
    }

    private void applyMask(final BitSet bitSet, final BitSet mask, final boolean maskValue) {
      for (int i = mask.nextSetBit(0); i >= 0; i = mask.nextSetBit(i + 1)) {
        if (i == Integer.MAX_VALUE) {
          break;
        }
        if (maskValue) {
          bitSet.set(i);
        } else {
          bitSet.clear(i);
        }
      }
    }
  }

  private static int parseMemoryAddress(final String line) {
    final int bracketBeginIndex = line.indexOf('[');
    final int bracketEndIndex = line.indexOf(']');
    if (bracketBeginIndex == -1 || bracketEndIndex == -1) {
      throw new RuntimeException();
    }
    return Integer.parseInt(line.substring(bracketBeginIndex + 1, bracketEndIndex));
  }

  private static BitSet parseValue(final String line) {
    final int equalsIndex = line.indexOf('=');
    if (equalsIndex == -1) {
      throw new RuntimeException();
    }
    final int value = Integer.parseInt(line.substring(equalsIndex + 2));
    return BitSet.valueOf(new long[]{value});
  }

  private static long sum(final Map<Integer, BitSet> memory) {
    long s = 0;
    for (final BitSet bitSet : memory.values()) {
      long l = 0;
      for (int i = 0; i < bitSet.length(); ++i) {
        l += bitSet.get(i) ? (1L << i) : 0L;
      }
      s += l;
    }
    return s;
  }

  public static void main(String[] args) {
    final Map<Integer, BitSet> memory = new HashMap<>();
    BitMask bitMask = null;

    final Scanner scanner = new Scanner(System.in);
    String line = scanner.nextLine();
    while (!line.isEmpty()) {
      if (line.startsWith("mask")) {
        bitMask = new BitMask(line.substring(7));
      } else if (line.startsWith("mem")) {
        final int memoryAddress = parseMemoryAddress(line);
        final BitSet value = parseValue(line);
        if (bitMask != null) {
          bitMask.apply(value);
          memory.put(memoryAddress, value);
        }
      } else {
        throw new RuntimeException();
      }
      line = scanner.nextLine();
    }
    System.out.println(sum(memory));
  }
}