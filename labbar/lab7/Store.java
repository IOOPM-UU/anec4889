public class Store {
    private Register registers[];

    public Store(int registersAmount) {
        this.registers = new Register[registersAmount];
        for (int i = 0; i < this.registers.length; i++) {
            this.registers[i] = new Register();
        }
        this.registers[0].open();
    }

    public int getAverageQueueLength() {
        int total = 0;
        int amount = 0;
        for (Register r : this.registers) {
            if (r.isOpen()) {
                total += r.getQueueLength();
                amount++;
            }
        }
        return (amount == 0) ? total : total / amount;
    }

    public void newCustomer(Customer c) {
        int shortest = 0;
        for (int i = 0; i < this.registers.length; i++) {
            if (this.registers[i].getQueueLength() < this.registers[shortest].getQueueLength()
                    && this.registers[i].isOpen()) {
                shortest = i;
            }
        }
        this.registers[shortest].addToQueue(c);
    }

    public void step() {
        for (Register r : this.registers) {
            if (r.currentCustomerIsDone()) {
                r.removeCurrentCustomer();
            }
            r.step();
        }
    }

    public void openNewRegister() {
        for (Register r : this.registers) {
            if (!r.isOpen()) {
                r.open();
                return;
            }
        }
        System.err.println("No registers left");
    }

    public Queue getDoneCustomers() {
        Queue customers = new Queue();
        for (Register r : this.registers) {
            if (r.currentCustomerIsDone()) {
                customers.enqueue(r.getFirstCustomer());
            }
        }
        return customers;
    }

    public int getLongestQueue() {
        int longest = 0;
        for (Register r : this.registers) {
            if (r.getQueueLength() > longest) {
                longest = r.getQueueLength();
            }
        }
        return longest;
    }

    public String toString() {
        String str = "";
        for (Register r : this.registers) {
            str += r.toString() + "\n";
        }
        return str;
    }
}