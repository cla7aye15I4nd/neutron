package com.example.neutron;

import android.annotation.SuppressLint;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.view.MotionEvent;
import android.view.View;
import android.widget.TextClock;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;

public class BuyTicketActivity extends AppCompatActivity {

    @SuppressLint("HandlerLeak")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_buy_ticket);
        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
            }
        });

        Intent intent = getIntent();
        final Train train = new Train(intent.getStringExtra("train"), null);

        ((TextView) findViewById(R.id.start_time_view)).setText(train.fromTime);
        ((TextView) findViewById(R.id.end_time_view)).setText(train.toTime);
        ((TextView) findViewById(R.id.start_point_view)).setText(train.from);
        ((TextView) findViewById(R.id.end_point_view)).setText(train.to);
        ((TextView) findViewById(R.id.date_view)).setText(train.fromDate);
        ((TextView) findViewById(R.id.trans_view)).setText(train.id);

        RecyclerView seat_view = findViewById(R.id.seat_view);
        LinearLayoutManager layoutManager = new LinearLayoutManager(this);
        seat_view.setLayoutManager(layoutManager);

        List<Option> option = new ArrayList<>();
        for (final Train.Seat seat : train.seats)
            option.add(new Option(seat.format(), new Handler() {
                @Override
                public void handleMessage(Message msg) {
                    super.handleMessage(msg);

                    SharedPreferences pref = getSharedPreferences("neutron_user", MODE_PRIVATE);
                    final String user_id = pref.getString("user_id", "0");
                    AlertDialog.Builder normalMoreButtonDialog = new AlertDialog.Builder(BuyTicketActivity.this);

                    if (user_id.equals("0")) {
                        normalMoreButtonDialog.setMessage("请先登陆");
                        normalMoreButtonDialog.setPositiveButton("确认",  new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                dialog.dismiss();
                                Intent intent = new Intent(BuyTicketActivity.this, LoginActivity.class);
                                startActivity(intent);
                            }
                        });
                    } else {
                        normalMoreButtonDialog.setTitle(getString(R.string.buy_ticket_tip_title));
                        normalMoreButtonDialog.setMessage("确认购买?");
                        normalMoreButtonDialog.setPositiveButton("确认", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                Toast.makeText(BuyTicketActivity.this, "确认", Toast.LENGTH_SHORT).show();
                                NeutronCommand.BuyTicket(user_id, "1", train.id, train.from, train.to,
                                        train.fromDate, seat.type, new Handler() {
                                            @Override
                                            public void handleMessage(Message msg) {
                                                super.handleMessage(msg);
                                                Bundle data = msg.getData();
                                                if (data.getInt("result") == -1) {
                                                    Toast.makeText(BuyTicketActivity.this,
                                                            "网络错误", Toast.LENGTH_SHORT).show();
                                                }else if (data.getInt("result") == 0) {
                                                    Toast.makeText(BuyTicketActivity.this,
                                                            "没票了", Toast.LENGTH_SHORT).show();
                                                }else{
                                                    Toast.makeText(BuyTicketActivity.this,
                                                                "购票成功", Toast.LENGTH_SHORT).show();
                                                }
                                            }
                                        });
                                dialog.dismiss();
                            }
                        });
                        normalMoreButtonDialog.setNegativeButton("取消", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                dialog.dismiss();
                            }
                        });
                    }
                    normalMoreButtonDialog.create().show();
                }

            }));

        NeutronAdapter adapter = new NeutronAdapter(option);
        seat_view.setAdapter(adapter);
    }
}
