import java.util.Scanner;

class Solution {

  private enum Direction {
    NORTH, EAST, SOUTH, WEST
  }

  private static final class Ship {

    private int longitude = 0;
    private int latitude = 0;
    private Direction direction = Direction.EAST;

    int getLongitude() {
      return longitude;
    }

    int getLatitude() {
      return latitude;
    }

    Direction getDirection() {
      return direction;
    }

    void moveForward(final int units) {
      translate(direction, units);
    }

    void translate(final Direction direction, final int units) {
      switch (direction) {
        case NORTH -> longitude += units;
        case SOUTH -> longitude -= units;
        case EAST -> latitude += units;
        case WEST -> latitude -= units;
      }
    }

    void turnLeft(final int degrees) {
      switch (degrees) {
        case 90 -> {
          switch (direction) {
            case NORTH -> direction = Direction.WEST;
            case SOUTH -> direction = Direction.EAST;
            case EAST -> direction = Direction.NORTH;
            case WEST -> direction = Direction.SOUTH;
          }
        }
        case 180 -> invertDirection();
        case 270 -> {
          switch (direction) {
            case NORTH -> direction = Direction.EAST;
            case SOUTH -> direction = Direction.WEST;
            case EAST -> direction = Direction.SOUTH;
            case WEST -> direction = Direction.NORTH;
          }
        }
        default ->
            throw new IllegalArgumentException(String.format("Invalid degrees=%d value", degrees));
      }
    }

    void turnRight(final int degrees) {
      switch (degrees) {
        case 90 -> {
          switch (direction) {
            case NORTH -> direction = Direction.EAST;
            case SOUTH -> direction = Direction.WEST;
            case EAST -> direction = Direction.SOUTH;
            case WEST -> direction = Direction.NORTH;
          }
        }
        case 180 -> invertDirection();
        case 270 -> {
          switch (direction) {
            case NORTH -> direction = Direction.WEST;
            case SOUTH -> direction = Direction.EAST;
            case EAST -> direction = Direction.NORTH;
            case WEST -> direction = Direction.SOUTH;
          }
        }
        default ->
            throw new IllegalArgumentException(String.format("Invalid degrees=%d value", degrees));
      }
    }

    private void invertDirection() {
      switch (direction) {
        case NORTH -> direction = Direction.SOUTH;
        case SOUTH -> direction = Direction.NORTH;
        case EAST -> direction = Direction.WEST;
        case WEST -> direction = Direction.EAST;
      }
    }
  }

  public static void main(String[] args) {
    final Scanner scanner = new Scanner(System.in);
    final Ship ship = new Ship();
    String line = scanner.nextLine();
    while (!line.isEmpty()) {
      final char actionId = line.charAt(0);
      final int units = Integer.parseInt(line.substring(1));
      switch (actionId) {
        case 'N' -> ship.translate(Direction.NORTH, units);
        case 'S' -> ship.translate(Direction.SOUTH, units);
        case 'E' -> ship.translate(Direction.EAST, units);
        case 'W' -> ship.translate(Direction.WEST, units);
        case 'L' -> ship.turnLeft(units);
        case 'R' -> ship.turnRight(units);
        case 'F' -> ship.moveForward(units);
      }
      line = scanner.nextLine();
    }
    System.out.println(Math.abs(ship.getLongitude()) + Math.abs(ship.getLatitude()));
  }
}
