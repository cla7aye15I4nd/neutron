package com.example.neutron.city;
import android.os.Handler;

import me.yokeyword.indexablerv.IndexableEntity;

/**
 * Created by YoKey on 16/10/7.
 */
public class CityEntity implements IndexableEntity {
    private long id;
    private String name;
    private String pinyin;
    public Handler handler;

    public CityEntity() {}
    public CityEntity(String name) {
        this.name = name;
    }
    public CityEntity(String name, Handler handler) {
        this.name = name;
        this.handler = handler;
    }
    public long getId() {
        return id;
    }
    public void setId(long id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }

    public String getPinyin() {
        return pinyin;
    }
    public void setPinyin(String pinyin) {
        this.pinyin = pinyin;
    }

    @Override
    public String getFieldIndexBy() {
        return name;
    }

    @Override
    public void setFieldIndexBy(String indexByField) {
        this.name = indexByField;
    }

    @Override
    public void setFieldPinyinIndexBy(String pinyin) {
        this.pinyin = pinyin;
    }
}
