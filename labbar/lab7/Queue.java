public class Queue {
    public class Node {
        private Customer element;
        private Node next;

        public Node(Customer c) {
            this.next = null;
            this.element = c;
        }
    }

    private Node first;
    private Node last;
    private int length;

    public Queue() {
        this.first = null;
        this.last = null;
        this.length = 0;
    }

    public int length() {
        return length;
    }

    public void enqueue(Customer c) {
        Node newNode = new Node(c);
        if (last == null) {
            first = newNode;
            last = newNode;
        } else {
            last.next = newNode;
            last = newNode;
        }
        length++;
    }

    public Customer dequeue() {
        if (first == null) {
            throw new IllegalArgumentException("Queue is empty!");
        }
        Customer c = first.element;
        first = first.next;
        if (first == null) {
            last = null;
        }
        length--;
        return c;
    }

    public Customer first() {
        return this.first.element;
    }

    public void printQueue() {
        Node current = first;
        while (current != null) {
            System.out.print(current.element + " ");
            current = current.next;
        }
        System.out.println();
    }
}