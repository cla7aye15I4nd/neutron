package com.example.neutron;

import android.os.Message;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;

public class TrainAdapter extends RecyclerView.Adapter<TrainAdapter.ViewHolder>{

    private List<Train> trains;

    static class ViewHolder extends RecyclerView.ViewHolder {
        View trainView;
        TextView trainName;

        private ViewHolder(View view) {
            super(view);
            trainView = view;
            trainName = view.findViewById(R.id.train_name);
        }
    }

    public TrainAdapter(List<Train> trains) {
        this.trains = trains;
    }

    @Override
    public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.train_item, parent, false);
        final ViewHolder holder = new ViewHolder(view);
        holder.trainView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Train train = trains.get(holder.getAdapterPosition());
                if (train.getHandler() == null)
                    Toast.makeText(v.getContext(), train.getName(), Toast.LENGTH_SHORT).show();
                else {
                    train.getHandler().sendMessage(new Message());
                }
            }
        });
        return holder;
    }

    @Override
    public void onBindViewHolder(ViewHolder holder, int position) {
        Train train = trains.get(position);
        holder.trainName.setText(train.getName());
    }

    @Override
    public int getItemCount() {
        return trains.size();
    }
}