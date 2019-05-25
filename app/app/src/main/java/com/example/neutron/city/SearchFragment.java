package com.example.neutron.city;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Filter;
import android.widget.Filterable;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.List;

import com.example.neutron.R;
import com.example.neutron.ToastUtil;

import static android.app.Activity.RESULT_OK;

/**
 * Demo: 搜索结果显示Fragment
 * Created by YoKey on 16/10/9.
 */
public class SearchFragment extends Fragment {
    private RecyclerView mRecyclerView;
    private TextView mTvNoResult;
    private SearchAdapter mAdapter;
    private List<CityEntity> mDatas;

    private String mQueryText;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_search_city, container, false);
        mTvNoResult = view.findViewById(R.id.tv_no_result);
        mRecyclerView = view.findViewById(R.id.recy);
        return view;
    }

    public void bindDatas(List<CityEntity> datas) {
        this.mDatas = datas;
        mAdapter = new SearchAdapter();
        mRecyclerView.setLayoutManager(new LinearLayoutManager(getActivity()));
        mRecyclerView.setHasFixedSize(true);
        mRecyclerView.setAdapter(mAdapter);
        if (mQueryText != null) {
            mAdapter.getFilter().filter(mQueryText);
        }
    }

    public void bindQueryText(String newText) {
        if (mDatas == null) {
            mQueryText = newText.toLowerCase();
        } else if (!TextUtils.isEmpty(newText)) {
            mAdapter.getFilter().filter(newText.toLowerCase());
        }
    }

    private class SearchAdapter extends RecyclerView.Adapter<SearchAdapter.VH> implements Filterable {
        private List<CityEntity> items = new ArrayList<>();

        public SearchAdapter() {
            items.clear();
            items.addAll(mDatas);
        }

        @Override
        public VH onCreateViewHolder(ViewGroup parent, int viewType) {
            final VH holder = new VH(LayoutInflater.from(getActivity()).inflate(R.layout.item_city, parent, false));
            holder.itemView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    int position = holder.getAdapterPosition();
                    String city = items.get(position).getName();
                    ToastUtil.showShort(getActivity(), "选择了" + city);

                    Handler handler = items.get(position).handler;
                    if (handler != null) {
                        Message msg = new Message();
                        Bundle data = new Bundle();

                        data.putString("city", city);
                        msg.setData(data);
                        handler.sendMessage(msg);
                    }
                }
            });
            return holder;
        }

        @Override
        public int getItemCount() {
            return items.size();
        }

        @Override
        public void onBindViewHolder(VH holder, int position) {
            holder.tvName.setText(items.get(position).getName());
        }

        @Override
        public Filter getFilter() {
            return new Filter() {
                @Override
                protected FilterResults performFiltering(CharSequence constraint) {
                    ArrayList<CityEntity> list = new ArrayList<>();
                    for (CityEntity item : mDatas) {
                        if (item.getPinyin().startsWith(constraint.toString()) || item.getName().contains(constraint)) {
                            list.add(item);
                        }
                    }
                    FilterResults results = new FilterResults();
                    results.count = list.size();
                    results.values = list;
                    return results;
                }

                @Override
                @SuppressWarnings("unchecked")
                protected void publishResults(CharSequence constraint, FilterResults results) {
                    ArrayList<CityEntity> list = (ArrayList<CityEntity>) results.values;
                    items.clear();
                    items.addAll(list);
                    if (results.count == 0) {
                        mTvNoResult.setVisibility(View.VISIBLE);
                    } else {
                        mTvNoResult.setVisibility(View.INVISIBLE);
                    }
                    notifyDataSetChanged();
                }
            };
        }

        class VH extends RecyclerView.ViewHolder {
            private TextView tvName;

            public VH(View itemView) {
                super(itemView);
                tvName = (TextView) itemView.findViewById(R.id.tv_name);
            }
        }
    }
}
