import java.rmi.Naming;
import java.util.Map;
import java.util.Scanner;

public class VotingClient {
    public static void main(String[] args) {
        try {
            VotingInterface stub = (VotingInterface) Naming.lookup("rmi://localhost:1099/VotingService");

            Scanner scanner = new Scanner(System.in);
            while (true) {
                System.out.println("\n1. Vote\n2. View Results\n3. Exit");
                System.out.print("Choose an option: ");
                int option = scanner.nextInt();
                scanner.nextLine();  // Consume newline

                switch (option) {
                    case 1:
                        System.out.print("Enter candidate name: ");
                        String name = scanner.nextLine();
                        String response = stub.vote(name);
                        System.out.println(response);
                        break;
                    case 2:
                        Map<String, Integer> results = stub.getResults();
                        System.out.println("\nVoting Results:");
                        results.forEach((candidate, votes) ->
                            System.out.println(candidate + ": " + votes + " vote(s)"));
                        break;
                    case 3:
                        System.out.println("Exiting.");
                        return;
                    default:
                        System.out.println("Invalid choice.");
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
