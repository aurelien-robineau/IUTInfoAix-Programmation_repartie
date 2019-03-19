import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class ServerTcpEcho {
    public static void main (String[] args) {

        try {
            Socket client;
            BufferedWriter out;
            BufferedReader in;

            ServerSocket serveur = new ServerSocket(50007);

            client = serveur.accept();

            in = new BufferedReader( new InputStreamReader(client.getInputStream()));
            String msg = in.readLine();

            out = new BufferedWriter(new OutputStreamWriter(client.getOutputStream()));
            out.write(msg);

            client.close();
            serveur.close();

        } catch (Exception e) {
            System.err.println(e.getMessage());
        }
    }
}
