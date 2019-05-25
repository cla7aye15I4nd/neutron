package com.example.neutron;

import android.view.View;
import android.os.Handler;

public class Option {
    private String name;
    private Handler handler;

    public Option (String name, Handler handler) {
        this.name = name;
        this.handler = handler;
    }
    public String getName() {
        return name;
    }
    public Handler getHandler() { return handler;  }
}
