package com.example.neutron;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.GridLayoutManager;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.StaggeredGridLayoutManager;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;

public class SettingActivity extends AppCompatActivity {
    @SuppressLint("HandlerLeak")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_setting);
        Toolbar toolbar = findViewById(R.id.toolbarSetting);
        setSupportActionBar(toolbar);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) { finish();  }
        });

        RecyclerView setting_view = findViewById(R.id.setting_view);
        LinearLayoutManager layoutManager = new LinearLayoutManager(this);
        setting_view.setLayoutManager(layoutManager);

        List<Option> option = new ArrayList<>();

        option.add(new Option("账号信息", null));
        option.add(new Option("主题设置", null));
        option.add(new Option("联系客服", null));
        if (!getSharedPreferences("neutron_user", MODE_PRIVATE).getString("user_id", "0").equals("0")) {
            option.add(new Option("登出账号", new Handler() {
                @Override
                public void handleMessage(Message msg) {
                    super.handleMessage(msg);
                    SharedPreferences.Editor editor = getSharedPreferences("neutron_user", MODE_PRIVATE).edit();
                    editor.putString("user_id", "0");
                    editor.apply();
                    setResult(RESULT_OK, new Intent());
                    finish();
                }
            }));
        }

        NeutronAdapter adapter = new NeutronAdapter(option);
        setting_view.setAdapter(adapter);
    }
}
