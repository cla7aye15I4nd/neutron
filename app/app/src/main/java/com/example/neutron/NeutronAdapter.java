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

public class NeutronAdapter extends RecyclerView.Adapter<NeutronAdapter.ViewHolder>{

    private List<Option> options;

    static class ViewHolder extends RecyclerView.ViewHolder {
        View optionView;
        TextView optionName;

        private ViewHolder(View view) {
            super(view);
            optionView = view;
            optionName = view.findViewById(R.id.option_name);
        }
    }

    public NeutronAdapter(List<Option> options) {
        this.options = options;
    }

    @Override
    public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.option_item, parent, false);
        final ViewHolder holder = new ViewHolder(view);
        holder.optionView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Option option = options.get(holder.getAdapterPosition());
                if (option.getHandler() == null)
                    Toast.makeText(v.getContext(), option.getName(), Toast.LENGTH_SHORT).show();
                else
                    option.getHandler().sendMessage(new Message());
            }
        });
        return holder;
    }

    @Override
    public void onBindViewHolder(ViewHolder holder, int position) {
        Option option = options.get(position);
        holder.optionName.setText(option.getName());
    }

    @Override
    public int getItemCount() {
        return options.size();
    }
}