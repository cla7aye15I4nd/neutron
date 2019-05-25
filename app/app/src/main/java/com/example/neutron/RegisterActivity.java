package com.example.neutron;

import android.content.Intent;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.util.regex.Pattern;

public class RegisterActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_register);
        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) { finish();  }
        });

        Button register_button = findViewById(R.id.register_now);
        register_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String username = ((EditText) findViewById(R.id.register_name)).getText().toString();
                String password = ((EditText) findViewById(R.id.register_password)).getText().toString();
                String confirm  = ((EditText) findViewById(R.id.register_confirm)).getText().toString();
                String email    = ((EditText) findViewById(R.id.register_email)).getText().toString();
                String phone    = ((EditText) findViewById(R.id.register_phone)).getText().toString();

                Pattern email_pattern = Pattern.compile("^(.+)@(.+)$");

                if (username.isEmpty())
                    Toast.makeText(RegisterActivity.this, "用户名不能为空", Toast.LENGTH_SHORT).show();
                else if (username.length() > 40)
                    Toast.makeText(RegisterActivity.this, "用户名太长", Toast.LENGTH_SHORT).show();
                else if (password.isEmpty())
                    Toast.makeText(RegisterActivity.this, "密码不能为空", Toast.LENGTH_SHORT).show();
                else if (username.length() > 20)
                    Toast.makeText(RegisterActivity.this, "密码太长", Toast.LENGTH_SHORT).show();
                else if (!confirm.equals(password))
                    Toast.makeText(RegisterActivity.this, "两次密码输入不一致", Toast.LENGTH_SHORT).show();
                else if (email.isEmpty())
                    Toast.makeText(RegisterActivity.this, "邮箱不能为空", Toast.LENGTH_SHORT).show();
                else if (email.length() > 20)
                    Toast.makeText(RegisterActivity.this, "邮箱太长", Toast.LENGTH_SHORT).show();
                else if (phone.isEmpty())
                    Toast.makeText(RegisterActivity.this, "手机不能为空", Toast.LENGTH_SHORT).show();
                else if (!email_pattern.matcher(email).matches())
                    Toast.makeText(RegisterActivity.this, "邮箱格式不正确", Toast.LENGTH_SHORT).show();
                else if (!phone.matches("^[0-9]+$"))
                    Toast.makeText(RegisterActivity.this, "手机格式不正确", Toast.LENGTH_SHORT).show();
                else if (phone.length() > 20)
                    Toast.makeText(RegisterActivity.this, "手机号太长", Toast.LENGTH_SHORT).show();
                else {
                    String UID = NeutronCommand.Register(username, password, email, phone);

                    if (UID.equals("-1")) {
                        Toast.makeText(RegisterActivity.this, "网络错误", Toast.LENGTH_SHORT).show();
                    } else {
                        Toast.makeText(RegisterActivity.this, "你的用户ID是" + UID, Toast.LENGTH_SHORT).show();

                        Intent intent = new Intent();
                        intent.putExtra("REGISTER_UID", UID);
                        setResult(RESULT_OK, intent);
                        finish();
                    }
                }
            }
        });
    }

}
