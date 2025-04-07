import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.concurrent.ConcurrentHashMap;
import java.util.Map;

public class VotingServer extends UnicastRemoteObject implements VotingInterface {

    private final Map<String, Integer> voteMap;

    protected VotingServer() throws RemoteException {
        super();
        voteMap = new ConcurrentHashMap<>();
    }

    public synchronized String vote(String candidate) throws RemoteException {
        voteMap.put(candidate, voteMap.getOrDefault(candidate, 0) + 1);
        return "Vote cast for: " + candidate;
    }

    public Map<String, Integer> getResults() throws RemoteException {
        return voteMap;
    }

    public static void main(String[] args) {
        try {
            VotingServer server = new VotingServer();
            Naming.rebind("rmi://localhost:1099/VotingService", server);
            System.out.println("Voting Server is running...");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
