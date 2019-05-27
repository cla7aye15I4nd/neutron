package com.example.neutron;

import com.example.neutron.QueryResultActivity;

import java.util.ArrayList;
import android.os.Handler;

public class Train {
    public String id;
    public String from, fromDate, fromTime;
    public String to, toDate, toTime;
    public ArrayList<Seat> seats;
    private Handler handler;

    public Train(String info, Handler handler) {
        String[] spice = info.split(" ");
        this.id = spice[0];
        this.from = spice[1];
        this.fromDate = spice[2];
        this.fromTime = spice[3];
        this.to = spice[4];
        this.toDate = spice[5];
        this.toTime = spice[6];

        this.seats = new ArrayList<>();
        for (int i = 7; i < spice.length; i += 3) {
            this.seats.add(new Seat(spice[i], spice[i + 1], spice[i + 2]));
        }

        this.handler = handler;
    }

    public String toString() {
        String result = id + " " + from + " " + fromDate + " " + fromTime + " " + to + " " + toDate + " " + fromTime;
        for (Seat seat : seats) {
            result = result + " " + seat.type + " " + seat.count + " " + seat.price;
        }
        return result;
    }

    public Handler getHandler() {
        return handler;
    }

    public String getName() {
        return String.format("%s : %s → %s", id.substring(5), fromTime, toTime);
    }

    public static class Seat{
        public String type, count, price;

        private Seat (String type, String count, String price) {
            this.type = type;
            this.count = count;
            this.price = price;
        }

        public String format() {
            return String.format("%s %s %s", type, "￥"+price, "剩余"+count+"张");
        }
    }
};