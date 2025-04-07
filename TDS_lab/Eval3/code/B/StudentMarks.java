import java.rmi.Remote;
import java.rmi.RemoteException;

public interface StudentMarks extends Remote {
    void addStudent(String name, int marks) throws RemoteException;
    int getMarks(String name) throws RemoteException;
    double getAverageMarks() throws RemoteException;
}
