import java.io.BufferedReader;  
import java.io.IOException;  
import java.io.InputStreamReader;  
import java.io.PrintStream;  
import java.net.Socket;  
import java.net.SocketTimeoutException;

public class NeutronClient{
    static private Socket client;
    static private PrintStream client_input;
    static private BufferedReader client_output;
    
    public static void connect(String host, int port) throws IOException {
	client = new Socket(host, port);
	client.setSoTimeout(2000);

	client_input = new PrintStream(client.getOutputStream());
	client_output = new BufferedReader(new InputStreamReader(client.getInputStream()));
    }

    public static void close() throws IOException { client.close(); }
    public static String send(String in) throws IOException {
	client_input.println(in);
	try{  
	    String echo = client_output.readLine();
	    return echo;
	}catch(SocketTimeoutException e){  
	    return "Time Out, No Respose";
	}
    }
    
    public static void main(String[] args) throws IOException {
	connect("193.112.69.6", 9999);

	BufferedReader input = new BufferedReader(new InputStreamReader(System.in));

	int count = 0;
	while (true) {
	    System.out.print("In[" + count + "]: ");
	    String in = input.readLine();
	    String out = send(in);
	    System.out.println("Out[" + count + "]: " + out);
	    count++;
	    if (out.equals("BYE"))
		break;
	}
	
	close();
    }
};
