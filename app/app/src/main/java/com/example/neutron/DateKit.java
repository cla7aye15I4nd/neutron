package com.example.neutron;

import android.util.Log;

import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Locale;

public class DateKit {
    public static final String YMD_FORMAT = "yyyy-MM-dd";
    public static final String YMD_FORMAT_2 = "yyyy.MM.dd";
    public static final String YM_FORMAT = "yyyy-MM";
    public static final String MD_FORMAT = "MM-dd";
    public static final String MD_FORMAT_2 = "MM.dd";
    public static final String HMS_FORMAT = "HH:mm:ss";
    public static final String HM_FORMAT = "HH:mm";
    public static final String YMD_HMS_FORMAT = "yyyy-MM-dd HH:mm:ss";
    public static final String YMD_HM_FORMAT = "yyyy-MM-dd HH:mm";
    public static final String YMD_HMS_FORMAT_2 = "yyyy年MM月dd日 HH:mm:ss";
    public static final String YMD_HM_FORMAT_2 = "yyyy年MM月dd日 HH:mm";
    public static final String YMDHMS_FORMAT = "yyyyMMddHHmmss";

    public static String date2String(Date date) {
        return date2String(date, null);
    }

    public static String date2String(Date date, String format) {
        if (null == format)
            format = YMD_HMS_FORMAT;
        SimpleDateFormat sdf = new SimpleDateFormat(format, Locale.getDefault());
        return sdf.format(date);
    }

    public static Date string2Date(String date, String format) {
        DateFormat inputDf = null;
        inputDf = new SimpleDateFormat(format, Locale.getDefault());

        Date result = null;
        try {
            result = inputDf.parse(date);
        } catch (ParseException e) {
            Log.d(e.toString(), date + " " + format);
        }
        return result;
    }

    public static Date getNow() {
        return new Date();
    }

    public static final int WEEKDAYS = 7;

    public static String[] WEEK = {"周日", "周一", "周二", "周三", "周四", "周五", "周六"};
    public static String[] WEEK_2 = {"星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"};

    public static String date2Week(Date date) {
        Calendar calendar = Calendar.getInstance();
        calendar.setTime(date);
        int dayIndex = calendar.get(Calendar.DAY_OF_WEEK);
        if (dayIndex < 1 || dayIndex > WEEKDAYS) {
            return null;
        }
        return WEEK[dayIndex - 1];
    }

    public static String formatDateTimeMill(long dateTime) {
        return formatDateTimeMill(dateTime, YMD_HMS_FORMAT);
    }

    public static String formatDateTimeMin(long dateTime) {
        return formatDateTimeMill(dateTime, YMD_HM_FORMAT);
    }

    public static String formatDateTimeMill(long dateTime, String format) {
        Date date = new Date(dateTime);
        SimpleDateFormat simpleDateFormat = new SimpleDateFormat(format, Locale.getDefault());
        return simpleDateFormat.format(date);
    }
}