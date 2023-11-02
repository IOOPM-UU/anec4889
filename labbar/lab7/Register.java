public class Register {
    private boolean open;
    private Queue queue;

    public Register() {
        this.open = false;
        this.queue = new Queue();
    }

    public void open() {
        this.open = true;
    }

    public void close() {
        this.open = false;
    }

    public boolean isOpen() {
        return this.open;
    }

    public void step() {
        if (this.queue.length() != 0)
            this.queue.first().serve();
    }

    public boolean hasCustomer() {
        return this.queue.length() != 0;
    }

    public boolean currentCustomerIsDone() {
        if (this.queue.length() != 0) {
            return this.queue.first().isDone();
        }
        return false;
    }

    public void addToQueue(Customer c) {
        this.queue.enqueue(c);
    }

    public Customer removeCurrentCustomer() {
        return this.queue.dequeue();
    }

    public int getQueueLength() {
        return this.queue.length();
    }

    public Customer getFirstCustomer() {
        return this.queue.first();
    }

    public String toString() {
        String str = "";
        if (this.isOpen()) {
            if (this.queue.length() != 0) {
                str += "    " + this.queue.first().toString();
                for (int i = 0; i < this.getQueueLength() - 1; i++) {
                    str += "@";
                }
            } else {
                str += "    [ ]";
            }
        } else {
            str += "X   [ ]";
        }
        return str;
    }
}