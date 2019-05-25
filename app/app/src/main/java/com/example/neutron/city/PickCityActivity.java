package com.example.neutron.city;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.annotation.Nullable;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.support.v7.widget.GridLayoutManager;
import android.support.v7.widget.SearchView;
import android.util.Log;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.FrameLayout;
import android.widget.Toast;

import com.github.promeg.pinyinhelper.Pinyin;
import com.github.promeg.tinypinyin.lexicons.android.cncity.CnCityDict;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;

import me.yokeyword.indexablerv.EntityWrapper;
import me.yokeyword.indexablerv.IndexableAdapter;
import me.yokeyword.indexablerv.IndexableHeaderAdapter;
import me.yokeyword.indexablerv.IndexableLayout;
import me.yokeyword.indexablerv.SimpleHeaderAdapter;

import com.example.neutron.R;
import com.example.neutron.ToastUtil;

public class PickCityActivity extends AppCompatActivity {
    private List<CityEntity> mData;
    private SearchFragment mSearchFragment;
    private SearchView mSearchView;
    private FrameLayout mProgressBar;

    private  void hiddenKeyBoard(View v) {
        InputMethodManager manager = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
        manager.hideSoftInputFromWindow(v.getWindowToken(), InputMethodManager.HIDE_NOT_ALWAYS);
    }

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_pick_city);

        mSearchFragment = (SearchFragment) getSupportFragmentManager().findFragmentById(R.id.search_fragment);
        IndexableLayout indexableLayout = findViewById(R.id.indexableLayout);
        mSearchView = findViewById(R.id.searchview);
        mProgressBar = findViewById(R.id.progress);

        indexableLayout.setLayoutManager(new GridLayoutManager(this, 2));

        Pinyin.init(Pinyin.newConfig().with(CnCityDict.getInstance(this)));

        //indexableLayout.setCompareMode(IndexableLayout.MODE_FAST);
        indexableLayout.setComparator(new Comparator<EntityWrapper<CityEntity>>() {
            @Override
            public int compare(EntityWrapper<CityEntity> lhs, EntityWrapper<CityEntity> rhs) {
                return lhs.getPinyin().compareTo(rhs.getPinyin());
            }
        });

        CityAdapter adapter = new CityAdapter(this);
        indexableLayout.setAdapter(adapter);

        mData = initDatas();
        adapter.setDatas(mData, new IndexableAdapter.IndexCallback<CityEntity>() {
            @Override
            public void onFinished(List<EntityWrapper<CityEntity>> data) {
                mSearchFragment.bindDatas(mData);
                mProgressBar.setVisibility(View.GONE);
            }
        });

        indexableLayout.setOverlayStyle_Center();

        adapter.setOnItemContentClickListener(new IndexableAdapter.OnItemContentClickListener<CityEntity>() {
            @Override
            public void onItemClick(View v, int originalPosition, int currentPosition, CityEntity entity) {
                if (originalPosition >= 0) {
                    Intent intent = new Intent();
                    intent.putExtra("city", entity.getName());
                    setResult(RESULT_OK, intent);
                    ToastUtil.showShort(PickCityActivity.this, "选中:" + entity.getName());
                    finish();
                } else
                    hiddenKeyBoard(v);
            }
        });

        adapter.setOnItemTitleClickListener(new IndexableAdapter.OnItemTitleClickListener() {
            @Override
            public void onItemClick(View v, int currentPosition, String indexTitle) { hiddenKeyBoard(v); }
        });

        SimpleHeaderAdapter mHotCityAdapter;
        mHotCityAdapter = new SimpleHeaderAdapter<>(adapter, "!", "热门", initHotCityDatas());
        mHotCityAdapter.setOnItemHeaderClickListener(new IndexableHeaderAdapter.OnItemHeaderClickListener<CityEntity>() {
            @Override
            public void onItemClick(View v, int currentPosition, CityEntity entity) {
                Intent intent = new Intent();
                intent.putExtra("city", entity.getName());
                setResult(RESULT_OK, intent);
                ToastUtil.showShort(PickCityActivity.this, "选中:" + entity.getName());
                finish();
            }
        });

        indexableLayout.addHeaderAdapter(mHotCityAdapter);

//        final List<CityEntity> gpsCity = iniyGPSCityDatas();
//        final SimpleHeaderAdapter gpsHeaderAdapter = new SimpleHeaderAdapter<>(adapter, "当", "当前城市", gpsCity);
//        indexableLayout.addHeaderAdapter(gpsHeaderAdapter);
//        indexableLayout.postDelayed(new Runnable() {
//            @Override
//            public void run() {
//                gpsCity.get(0).setName("杭州市");
//                gpsHeaderAdapter.notifyDataSetChanged();
//            }
//        }, 300);

        initSearch();
    }

    private List<CityEntity> initDatas() {
        List<CityEntity> list = new ArrayList<>();
        List<String> cityStrings = Arrays.asList(getResources().getStringArray(R.array.city_array));
        for (String item : cityStrings) {
            @SuppressLint("HandlerLeak") Handler handler = new Handler() {
                @Override
                public void handleMessage(Message msg) {
                    super.handleMessage(msg);
                    Bundle data = msg.getData();
                    Intent intent = new Intent();
                    intent.putExtra("city", data.getString("city"));
                    setResult(RESULT_OK, intent);
                    finish();
                }
            };
            list.add(new CityEntity(item, handler));
        }
        return list;
    }

    private List<CityEntity> initHotCityDatas() {
        List<CityEntity> list = new ArrayList<>();
        list.add(new CityEntity("长沙南"));
        list.add(new CityEntity("郑州东"));
        list.add(new CityEntity("上海虹桥"));
        list.add(new CityEntity("北京西"));
        return list;
    }

    private List<CityEntity> iniyGPSCityDatas() {
        List<CityEntity> list = new ArrayList<>();
        list.add(new CityEntity("定位中..."));
        return list;
    }

    private void initSearch() {
        getSupportFragmentManager().beginTransaction().hide(mSearchFragment).commit();

        mSearchView.setOnQueryTextListener(new SearchView.OnQueryTextListener() {
            @Override
            public boolean onQueryTextSubmit(String query) {
                return false;
            }

            @Override
            public boolean onQueryTextChange(String newText) {
                if (newText.trim().length() > 0) {
                    if (mSearchFragment.isHidden()) {
                        getSupportFragmentManager().beginTransaction().show(mSearchFragment).commit();
                    }
                } else {
                    if (!mSearchFragment.isHidden()) {
                        getSupportFragmentManager().beginTransaction().hide(mSearchFragment).commit();
                    }
                }

                mSearchFragment.bindQueryText(newText);
                return false;
            }
        });
    }

    @Override
    public void onBackPressed() {
        if (!mSearchFragment.isHidden()) {
            mSearchView.setQuery(null, false);
            getSupportFragmentManager().beginTransaction().hide(mSearchFragment).commit();
            return;
        }
        super.onBackPressed();
    }
}