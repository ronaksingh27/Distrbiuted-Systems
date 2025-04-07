import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Scanner;

public class Client {
    public static void main(String[] args) {
        try {
            Registry registry = LocateRegistry.getRegistry("localhost");
            StudentMarks stub = (StudentMarks) registry.lookup("StudentMarksService");

            Scanner scanner = new Scanner(System.in);
            while (true) {
                System.out.println("\n1. Add Student\n2. Get Marks\n3. Get Average\n4. Exit");
                System.out.print("Choose option: ");
                int choice = scanner.nextInt();
                scanner.nextLine();

                switch (choice) {
                    case 1:
                        System.out.print("Enter name: ");
                        String name = scanner.nextLine();
                        System.out.print("Enter marks: ");
                        int marks = scanner.nextInt();
                        stub.addStudent(name, marks);
                        System.out.println("Student added.");
                        break;
                    case 2:
                        System.out.print("Enter name: ");
                        name = scanner.nextLine();
                        int studentMarks = stub.getMarks(name);
                        if (studentMarks == -1) {
                            System.out.println("Student not found.");
                        } else {
                            System.out.println("Marks: " + studentMarks);
                        }
                        break;
                    case 3:
                        double avg = stub.getAverageMarks();
                        System.out.println("Average Marks: " + avg);
                        break;
                    case 4:
                        System.exit(0);
                    default:
                        System.out.println("Invalid option.");
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
