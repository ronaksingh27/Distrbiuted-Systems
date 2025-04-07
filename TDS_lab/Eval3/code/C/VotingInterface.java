import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.Map;

public interface VotingInterface extends Remote {
    String vote(String candidate) throws RemoteException;
    Map<String, Integer> getResults() throws RemoteException;
}
