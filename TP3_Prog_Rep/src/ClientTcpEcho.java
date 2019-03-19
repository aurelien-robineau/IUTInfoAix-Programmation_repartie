import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.InetSocketAddress;
import java.net.Socket;

public class ClientTcpEcho {
    public static void main (String[] args) {

        try {
            // Lecture console
            System.out.print("Saisissez une chaîne : ");
            BufferedReader consoleBR = new BufferedReader( new InputStreamReader(System.in) );

            // Socket client
            Socket sockClient = new Socket();
            sockClient.connect( new InetSocketAddress("10.203.9.145", 50007) );

            BufferedWriter out = new BufferedWriter( new OutputStreamWriter(sockClient.getOutputStream()) );

            String ligne;
            BufferedReader in = new BufferedReader(new InputStreamReader(sockClient.getInputStream()));

            while (true) {
                String input = consoleBR.readLine();
                if(input.equals("quit")) {
                    break;
                } else {
                    out.write(input);
                    out.newLine();
                    out.flush();

                    String received = in.readLine();
                    System.out.println(received);
                }
            }

            out.close();
            in.close();
            sockClient.close();

        } catch (Exception e) {
            System.err.println(e.getMessage());
        }
    }
}
