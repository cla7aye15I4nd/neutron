package com.example.neutron;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;

public class QueryResultActivity extends AppCompatActivity {
    @SuppressLint("HandlerLeak")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_query_result);
        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) { finish();  }
        });

        Intent intent = getIntent();
        String command = intent.getStringExtra("command");

        final List<Train> trains = new ArrayList<>();
        final RecyclerView query_view = findViewById(R.id.train_view);
        query_view.setLayoutManager(new LinearLayoutManager(this));

        NeutronCommand.QueryTicket(command, new Handler() {
            @Override
            public void handleMessage(Message msg) {
                super.handleMessage(msg);
                Bundle data = msg.getData();

                int count = data.getInt("ticket_count");
                if (count == -2)
                    Toast.makeText(QueryResultActivity.this, "网络错误", Toast.LENGTH_SHORT).show();
                else if (count <= 0)
                    Toast.makeText(QueryResultActivity.this, "无直达列车", Toast.LENGTH_SHORT).show();
                else {
                    for (final String train : data.getStringArrayList("ticket_data"))
                        trains.add(new Train(train, new Handler() {
                            @Override
                            public void handleMessage(Message msg) {
                                super.handleMessage(msg);
                                Intent intent = new Intent(QueryResultActivity.this, BuyTicketActivity.class);
                                intent.putExtra("train", train);
                                startActivity(intent);
                            }
                        }));
                    TrainAdapter adapter = new TrainAdapter(trains);
                    query_view.setAdapter(adapter);
                }
            }
        });
    }
}
