import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class Server {
    public static void main(String[] args) {
        try {
            StudentMarksImpl obj = new StudentMarksImpl();
            Registry registry = LocateRegistry.createRegistry(1099);
            registry.rebind("StudentMarksService", obj);
            System.out.println("Server is ready.");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
