package com.example.neutron;

import android.annotation.SuppressLint;
import android.app.DatePickerDialog;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.StrictMode;
import android.view.MotionEvent;
import android.view.View;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.view.animation.Animation;
import android.view.animation.RotateAnimation;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.DatePicker;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import com.example.neutron.city.PickCityActivity;

import java.util.Calendar;

public class MainActivity extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener {

    final RotateAnimation rotateAnimation = new RotateAnimation(
            0, 1800,
            Animation.RELATIVE_TO_SELF, 0.5f,
            Animation.RELATIVE_TO_SELF, 0.5f);

    @SuppressLint("ClickableViewAccessibility")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy);

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        DrawerLayout drawer = findViewById(R.id.drawer_layout);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawer.addDrawerListener(toggle);
        toggle.syncState();

        NavigationView navigationView = findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);

        Button switch_button = findViewById(R.id.switch_button);
        final Button start_address = findViewById(R.id.start_button);
        final Button end_address   = findViewById(R.id.end_button);

        start_address.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivityForResult(new Intent(MainActivity.this, PickCityActivity.class), 3);
            }
        });

        start_address.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if(event.getAction() == MotionEvent.ACTION_UP)
                    start_address.setTextColor(getResources().getColor(R.color.colorBlack));
                if(event.getAction() == MotionEvent.ACTION_DOWN)
                    start_address.setTextColor(getResources().getColor(R.color.colorBlue));
                return false;
            }
        });

        end_address.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivityForResult(new Intent(MainActivity.this, PickCityActivity.class), 4);
            }
        });

        end_address.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if(event.getAction() == MotionEvent.ACTION_UP)
                    end_address.setTextColor(getResources().getColor(R.color.colorBlack));
                if(event.getAction() == MotionEvent.ACTION_DOWN)
                    end_address.setTextColor(getResources().getColor(R.color.colorBlue));
                return false;
            }
        });

        switch_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                rotateAnimation.setDuration(800);
                v.startAnimation(rotateAnimation);

                String start = start_address.getText().toString();
                String end   = end_address.getText().toString();
                start_address.setText(end);
                end_address.setText(start);
            }
        });

        final Button time_button   = findViewById(R.id.time_button);

        final Calendar calendar = Calendar.getInstance();
        final DatePickerDialog dialog = new DatePickerDialog(MainActivity.this,
            new DatePickerDialog.OnDateSetListener() {
                @Override
                public void onDateSet(DatePicker view, int year, int month, int dayOfMonth) {
                    calendar.set(Calendar.YEAR, year);
                    calendar.set(Calendar.MONTH, month);
                    calendar.set(Calendar.DAY_OF_MONTH, dayOfMonth);
                    time_button.setText(DateKit.date2String(calendar.getTime(), "yyyy-MM-dd"));
                }
            },
            calendar.get(Calendar.YEAR),
            calendar.get(Calendar.MONTH),
            calendar.get(Calendar.DAY_OF_MONTH));

        time_button.setText(DateKit.date2String(DateKit.getNow(), "yyyy-MM-dd"));
        time_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {  dialog.show(); }
        });

        final CheckBox checkBox_T = findViewById(R.id.train_type_T);
        final CheckBox checkBox_Z = findViewById(R.id.train_type_Z);
        final CheckBox checkBox_O = findViewById(R.id.train_type_O);
        final CheckBox checkBox_G = findViewById(R.id.train_type_G);
        final CheckBox checkBox_D = findViewById(R.id.train_type_D);
        final CheckBox checkBox_K = findViewById(R.id.train_type_K);
        final CheckBox checkBox_C = findViewById(R.id.train_type_C);
        final CheckBox checkBox_A = findViewById(R.id.train_type_A);

        checkBox_A.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                checkBox_T.setChecked(isChecked);
                checkBox_Z.setChecked(isChecked);
                checkBox_O.setChecked(isChecked);
                checkBox_G.setChecked(isChecked);
                checkBox_D.setChecked(isChecked);
                checkBox_C.setChecked(isChecked);
                checkBox_K.setChecked(isChecked);
            }
        });

        Button query_button = findViewById(R.id.main_query_button);
        query_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, QueryResultActivity.class);

                String command = "query_ticket " +
                        start_address.getText() +  " " +
                        end_address.getText() + " " +
                        time_button.getText() + " ";
                int len = command.length();
                if (checkBox_T.isChecked()) command += "T";
                if (checkBox_Z.isChecked()) command += "Z";
                if (checkBox_O.isChecked()) command += "O";
                if (checkBox_G.isChecked()) command += "G";
                if (checkBox_D.isChecked()) command += "D";
                if (checkBox_C.isChecked()) command += "C";
                if (checkBox_K.isChecked()) command += "K";

                if (command.length() > len) {
                    intent.putExtra("command", command);
                    startActivityForResult(intent, 4);
                }
                else Toast.makeText(MainActivity.this, "请选择车次类型", Toast.LENGTH_SHORT).show();
            }
        });
    }

    @Override
    public void onBackPressed() {
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        if (drawer.isDrawerOpen(GravityCompat.START)) drawer.closeDrawer(GravityCompat.START);
        else super.onBackPressed();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        refreshUserInformation();

        ImageButton image_button = findViewById(R.id.image_login);
        image_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                SharedPreferences pref = getSharedPreferences("neutron_user", MODE_PRIVATE);
                if (!pref.getString("user_id", "0").equals("0"))
                    onInfo();
                else
                    onLogin();
            }
        });


        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement

        return super.onOptionsItemSelected(item);
    }

    protected void refreshUserInformation() {
        SharedPreferences pref = getSharedPreferences("neutron_user", MODE_PRIVATE);
        String user_id = pref.getString("user_id", "0");

        if (!user_id.equals("0")) {
            @SuppressLint("HandlerLeak") Handler query_handler = new Handler() {
                @Override
                public void handleMessage(Message msg) {
                    super.handleMessage(msg);
                    Bundle data = msg.getData();

                    if (data.getString("result").equals("1")) {
                        SharedPreferences.Editor editor = getSharedPreferences("neutron_user", MODE_PRIVATE).edit();
                        editor.putString("name", data.getString("name"));
                        editor.putString("email", data.getString("email"));
                        editor.putString("phone", data.getString("phone"));
                        editor.putString("privilege", data.getString("privilege"));
                        editor.apply();

                        TextView name_view = findViewById(R.id.name_view);
                        TextView email_view = findViewById(R.id.email_view);
                        name_view.setText(data.getString("name"));
                        email_view.setText(data.getString("email"));
                    } else Toast.makeText(MainActivity.this, "网络错误", Toast.LENGTH_SHORT).show();
                }
            };
            NeutronCommand.QueryProfile(user_id, query_handler);
        } else {
            TextView name_view = findViewById(R.id.name_view);
            TextView email_view = findViewById(R.id.email_view);
            name_view.setText(R.string.nav_header_title);
            email_view.setText(R.string.nav_header_subtitle);
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case 1:
                if (resultCode == RESULT_OK)
                    refreshUserInformation();
                break;
            case 2:
                if (resultCode == RESULT_OK)
                    refreshUserInformation();
                break;
            case 3:
                if (resultCode == RESULT_OK) {
                    String start = data.getStringExtra("city");
                    TextView start_view = findViewById(R.id.start_button);
                    start_view.setText(start);
                }
                break;
            case 4:
                if (resultCode == RESULT_OK) {
                    String start = data.getStringExtra("city");
                    TextView start_view = findViewById(R.id.end_button);
                    start_view.setText(start);
                }
            default:
                refreshUserInformation();
        }
    }

    protected void onLogin() {
        Intent intent = new Intent(MainActivity.this, LoginActivity.class);
        startActivityForResult(intent, 1);
    }

    protected void onSetting() {
        Intent intent = new Intent(MainActivity.this, SettingActivity.class);
        startActivityForResult(intent, 2);
    }

    protected void onInfo() {
        Intent intent = new Intent(MainActivity.this, InfoActivity.class);
        startActivityForResult(intent, 3);
    }

    @SuppressWarnings("StatementWithEmptyBody")
    @Override
    public boolean onNavigationItemSelected(MenuItem item) {
        // Handle navigation view item clicks here.
        int id = item.getItemId();

        if (id == R.id.nav_index) {
        } else if (id == R.id.nav_booking) {
        } else if (id == R.id.nav_ticket) {
        } else if (id == R.id.nav_theme) {
        } else if (id == R.id.nav_setting) onSetting();

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);
        return true;
    }
}
