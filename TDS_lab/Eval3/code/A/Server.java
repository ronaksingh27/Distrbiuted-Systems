// Server.java
import java.rmi.Naming;
import java.rmi.registry.LocateRegistry;

public class Server {
    public static void main(String[] args) {
        try {
            LocateRegistry.createRegistry(1099); // Start RMI registry on port 1099
            Calculator calc = new CalculatorImpl();
            Naming.rebind("CalculatorService", calc);
            System.out.println("Calculator Service is running...");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
