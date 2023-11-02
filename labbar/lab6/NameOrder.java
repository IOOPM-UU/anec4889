import java.util.Scanner;

public class NameOrder {

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Name 1: ");
        String name1 = sc.nextLine().toLowerCase();
        Scanner sc2 = new Scanner(System.in);
        System.out.print("Name 2: ");
        String name2 = sc2.nextLine().toLowerCase();

        if (name1.compareTo(name2) <= 0) {
            System.out.println(name1 + "\n" + name2);
        } else {
            System.out.println(name2 + "\n" + name1);
        }
    }
}