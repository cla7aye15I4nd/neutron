package com.example.neutron;

import java.io.IOException;
import java.net.ConnectException;
import java.net.SocketTimeoutException;
import java.util.ArrayList;
import java.util.List;

import android.os.Handler;
import android.os.Bundle;
import android.os.Message;
import android.util.Log;


public class NeutronCommand{
    private static NeutronClient getDatabase() throws IOException {
        return new NeutronClient();
    }

    public static void BuyTicket(final String id, final String num, final String train_id,
                                 final String from, final String to, final String date,
                                 final String kind, final Handler handler) {
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                Message msg = new Message();
                Bundle data = new Bundle();
                String command = String.format("buy_ticket %s %s %s %s %s %s %s",
                        id, num, train_id, from, to, date, kind);
                try {
                    NeutronClient db = getDatabase();
                    data.putInt("result", Integer.parseInt(db.send_close(command)));
                } catch (IOException e) {
                    data.putInt("result", -1);
                }
                msg.setData(data);
                handler.sendMessage(msg);
            }
        };
        new Thread(runnable).start();
    }

    public static void QueryTicket(final String command, final Handler handler) {
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                Message msg = new Message();
                Bundle data = new Bundle();
                try {
                    NeutronClient db = getDatabase();
                    db.send(command);

                    int count = Integer.parseInt(db.getLine());
                    data.putInt("ticket_count", count);

                    ArrayList<String> tickets = new ArrayList<>();
                    while (count > 0) {
                        tickets.add(db.getLine());
                        count--;
                    }

                    data.putStringArrayList("ticket_data", tickets);
                } catch (SocketTimeoutException e) {
                    data.putInt("ticket_count", -1);
                } catch (IOException e) {
                    data.putInt("ticket_count", -2);
                }

                msg.setData(data);
                handler.sendMessage(msg);
            }
        };
        new Thread(runnable).start();
    }
    public static void LoginVerify(final String user_id, final String password, final Handler handler) {
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                Message msg = new Message();
                Bundle data = new Bundle();
                Log.d("exe", "start");
                try {
                    data.putString("user_id", user_id);
                    data.putString("password", password);
                    data.putString("result", getDatabase().send_close("login " + user_id + " " + password));
                } catch (SocketTimeoutException timeout) {
                    data.putString("result", "-1");
                } catch (IOException io) {
                    data.putString("result", "-1");
                }
                Log.d("exe", "start2");
                msg.setData(data);
                handler.sendMessage(msg);
            }
        };
        new Thread(runnable).start();
    }

    public static void QueryProfile(final String user_id, final Handler handler) {
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                Message msg = new Message();
                Bundle editor = new Bundle();

                try {
                    String info = getDatabase().send_close("query_profile " + user_id);
                    String[] user_information = info.split(" ");

                    if (user_information.length == 4) {
                        editor.putString("result", "1");
                        editor.putString("name", user_information[0]);
                        editor.putString("email", user_information[1]);
                        editor.putString("phone", user_information[2]);
                        editor.putString("privilege", user_information[3]);
                    } else
                        editor.putString("result", "0");
                } catch (SocketTimeoutException e) {
                    editor.putString("result", "-1");
                } catch (IOException e) {
                    editor.putString("result", "-1");
                }
                msg.setData(editor);
                handler.sendMessage(msg);
            }
        };
        new Thread(runnable).start();
    }

    public static String Register(String name, String password, String email, String phone) {
        try {
            return getDatabase().send_close("register " + name + " " + password + " " + email + " " + phone);
        } catch (SocketTimeoutException e) {
            return "-1";
        } catch (IOException e) {
            return "-1";
        }
    }
    
};
