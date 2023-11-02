import java.util.Scanner;

public class Die {
  private int numberOfSides;
  private int value;

  public Die() {
    this.numberOfSides = 6;
  }

  public Die(int numberOfSides) {
    this.numberOfSides = numberOfSides;
  }

  public int roll() {
    this.value = (int) (Math.random() * numberOfSides) + 1;
    return this.get();
  }

  public int get() {
    return value;
  }

  public String toString() {
    return "Die(" + value + ")";
  }

  public boolean equals(Die otherDie) {
    if (this == otherDie)
      return true;
    return otherDie.value == this.value && otherDie.numberOfSides == this.numberOfSides;
  }

  public static void main(String[] args) {
    Scanner sc = new Scanner(System.in);
    System.out.print("Number of sides: ");
    int sides = sc.nextInt();
    if (sides < 1) {
      throw new IllegalArgumentException(sides + " is an illegal number of sides for die");
    }
    Die d = new Die(sides);
    Die s = new Die(3);
    System.out.println(d.equals(s));
    System.out.println("Alea iacta est: " + d.roll());
  }
}
