package com.example.neutron;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.TextView;

import com.example.neutron.R;

public class InfoActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_info);
        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) { finish();  }
        });

        SharedPreferences pref = getSharedPreferences("neutron_user", MODE_PRIVATE);
        String name = pref.getString("name", "0");
        String uid = pref.getString("user_id", "0");
        String email = pref.getString("email", "0");
        String phone = pref.getString("phone", "0");
        String privilege = pref.getString("privilege", "0");

        ((TextView) findViewById(R.id.textName)).setText(name);
        ((TextView) findViewById(R.id.textUID)).setText(uid);
//        ((TextView) findViewById(R.id.textEmail)).setText(email);
//        ((TextView) findViewById(R.id.textPhone)).setText(phone);
//        ((TextView) findViewById(R.id.textPri)).setText(privilege);
    }

}
