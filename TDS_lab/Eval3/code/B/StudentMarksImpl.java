import java.rmi.server.UnicastRemoteObject;
import java.rmi.RemoteException;
import java.util.HashMap;
import java.util.Map;

public class StudentMarksImpl extends UnicastRemoteObject implements StudentMarks {
    private Map<String, Integer> studentData;

    public StudentMarksImpl() throws RemoteException {
        studentData = new HashMap<>();
    }

    public void addStudent(String name, int marks) throws RemoteException {
        studentData.put(name, marks);
    }

    public int getMarks(String name) throws RemoteException {
        return studentData.getOrDefault(name, -1);
    }

    public double getAverageMarks() throws RemoteException {
        if (studentData.isEmpty()) return 0.0;
        int total = 0;
        for (int marks : studentData.values()) {
            total += marks;
        }
        return (double) total / studentData.size();
    }
}
