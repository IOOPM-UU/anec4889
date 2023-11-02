import java.util.Scanner;

public class PairOfDice {
    private Die one;
    private Die two;

    public PairOfDice() {
        this.one = new Die();
        this.two = new Die();
    }

    public String RollPair() {
        this.one.roll();
        this.two.roll();
        return "(" + this.one.get() + ", " + this.two.get() + ")";
    }

    public String toStringPair() {
        return "Die 1(" + one.get() + "), Die 2(" + two.get() + ")";
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        PairOfDice d = new PairOfDice();
        System.out.println(d.RollPair());
        System.out.println(d.RollPair());
        System.out.println(d.RollPair());

        System.out.println(d.one.get());
        System.out.println(d.two.get());
        System.out.println(d.toStringPair());
    }
}