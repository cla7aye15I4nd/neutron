package com.example.neutron;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintStream;
import java.net.ConnectException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.net.SocketTimeoutException;
import android.util.Log;

public class NeutronClient{
    private Socket client;
    private PrintStream client_input;
    private BufferedReader client_output;

    private final static int port = 9999;
    private final static String host = "193.112.69.6";

    NeutronClient () throws IOException { connect(); }

    private void connect() throws IOException {
        client = new Socket();
        client.connect(new InetSocketAddress(host, port), 2000);
        client.setSoTimeout(2000);

        client_input = new PrintStream(client.getOutputStream());
        client_output = new BufferedReader(new InputStreamReader(client.getInputStream(),"utf-8"));
    }

    private void close() {
        try {
            client.close();
        } catch (IOException e) {
            Log.d("close", "IOException");
        }
    }

    public void send(String in) {
        client_input.println(in);
    }

    public String getLine() throws SocketTimeoutException, IOException{
        return client_output.readLine();
    }

    public String send_close(String in) throws SocketTimeoutException, IOException{
        send(in);
        String ret = getLine();
        close();
        return ret;
    }
};
