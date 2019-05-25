package com.example.neutron;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.StrictMode;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v4.app.NavUtils;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;


public class LoginActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy);

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        Toolbar toolbar = findViewById(R.id.toolbarLogin);
        setSupportActionBar(toolbar);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) { finish();  }
        });

        Button register_button = findViewById(R.id.register_button);
        register_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(LoginActivity.this, RegisterActivity.class);
                startActivityForResult(intent, 1);
            }
        });

        Button login_button = findViewById(R.id.login_button);
        login_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String username = ((EditText) findViewById(R.id.login_username)).getText().toString();
                String password = ((EditText) findViewById(R.id.login_password)).getText().toString();

                if (username.isEmpty())
                    Toast.makeText(LoginActivity.this, "用户ID不能为空", Toast.LENGTH_SHORT).show();
                else if (username.length() > 20)
                    Toast.makeText(LoginActivity.this, "用户ID不存在", Toast.LENGTH_SHORT).show();
                else if (password.isEmpty())
                    Toast.makeText(LoginActivity.this, "密码不能为空", Toast.LENGTH_SHORT).show();
                else if (password.length() > 20)
                    Toast.makeText(LoginActivity.this, "用户名或密码错误", Toast.LENGTH_SHORT).show();
                else {
                     @SuppressLint("HandlerLeak") Handler login_handler = new Handler() {
                        @Override
                        public void handleMessage(Message msg) {
                            super.handleMessage(msg);
                            Bundle data = msg.getData();
                            String val = data.getString("result");

                            Log.d("exe", "error");
                            if (val.equals("1")) {
                                Toast.makeText(LoginActivity.this, "登陆成功", Toast.LENGTH_SHORT).show();
                                SharedPreferences.Editor editor = getSharedPreferences("neutron_user", MODE_PRIVATE).edit();

                                editor.putString("user_id", data.getString("user_id"));
                                editor.putString("password", data.getString("password"));
                                editor.apply();

                                Intent intent = new Intent();
                                intent.putExtra("LOGIN_STATE", true);
                                setResult(RESULT_OK, intent);
                                finish();
                            } else if (val.equals("-1"))
                                Toast.makeText(LoginActivity.this, "网络错误", Toast.LENGTH_SHORT).show();
                            else
                                Toast.makeText(LoginActivity.this, "用户名或密码错误", Toast.LENGTH_SHORT).show();
                        }
                    };

                    NeutronCommand.LoginVerify(username, password, login_handler);
                }
            }
        });
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case 1:
                if (resultCode == RESULT_OK) {
                    EditText name_edit = findViewById(R.id.login_username);
                    name_edit.setText(data.getStringExtra("REGISTER_UID"));
                }
                break;
            default:
        }
    }
}
