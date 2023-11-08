import java.util.Random;
import java.lang.Math;

public class Simulation {
    private Store store;
    private int time;
    private int intensity;
    private int maxGroceries;
    private int thresholdForNewRegister;
    private int doneCustomers;
    private int averageWaitTime;

    public Simulation(int intensity, int maxGroceries, int thresholdForNewRegister) {
        this.store = new Store(4);
        this.time = 0;
        this.intensity = intensity;
        this.maxGroceries = maxGroceries;
        this.thresholdForNewRegister = thresholdForNewRegister;
        this.doneCustomers = 0;
        this.averageWaitTime = 0;
    }

    public void step() {
        this.time++;
        Random randomIntensity = new Random();
        if (randomIntensity.nextInt(100) < this.intensity) {
            Random randomGroceries = new Random();
            Customer c = new Customer(randomGroceries.nextInt(this.maxGroceries) + 1, this.time);
            this.store.newCustomer(c);
        }
        if (this.store.getAverageQueueLength() > this.thresholdForNewRegister) {
            this.store.openNewRegister();
        }
        this.store.step();
    }

    public String toString() {
        String str = this.store.toString() + "\n";

        Queue done = this.store.getDoneCustomers();
        this.doneCustomers += done.length();
        str += "Number of customers served: " + this.doneCustomers + "\n";
        str += "Max wait time: " + this.store.getLongestQueue() + "\n";

        for (int i = 0; i < done.length(); i++) {
            Customer c = done.dequeue();
            this.averageWaitTime += this.time - c.getBornTime();
        }

        str += "Average wait time: ";
        if (this.doneCustomers == 0)
            str += this.averageWaitTime;
        else {
            str += (float) this.averageWaitTime / this.doneCustomers;
        }
        return str;
    }

    public static void main(String[] args) throws InterruptedException {
        int steps = 100;
        int intensity = 50;
        int maxGroceries = 5;
        int thresholdForNewRegister = 2;
        Simulation s = new Simulation(intensity, maxGroceries, thresholdForNewRegister);
        for (int i = 0; i < steps; i++) {
            System.out.print("\033[2J\033[;H");
            s.step();
            System.out.println(s.toString());
            Thread.sleep(300);
        }
        System.out.println("");
        System.out.println("Simulation finished!");
    }
}