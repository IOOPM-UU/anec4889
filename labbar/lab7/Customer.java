import java.util.Scanner;

public class Customer {
    private int bornTime;
    private int groceries;

    public Customer(int groceries, int bornTime) {
        this.bornTime = bornTime;
        this.groceries = groceries;
    }

    public int serve() {
        if (this.groceries > 0)
            this.groceries -= 1;
        return this.groceries;
    }

    public boolean isDone() {
        return this.groceries == 0;
    }

    public int getBornTime() {
        return bornTime;
    }

    public String toString() {
        return "[" + this.groceries + "]";
    }

}