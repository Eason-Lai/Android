package com.example.liyixun.calculator;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import java.util.Stack;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    private String str;
    private EditText Edt;
    private TextView Tev;
    private float sum, n1, n2,t; //sum结果，n1整数乘积,n2小数乘机，t当前按下的数字；
    private Stack<Float> sn = new Stack<>(); //存数字
    private Stack<Integer> ss = new Stack<>(); //存符号
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        findViewById(R.id.btn_0).setOnClickListener(this);
        findViewById(R.id.btn_1).setOnClickListener(this);
        findViewById(R.id.btn_2).setOnClickListener(this);
        findViewById(R.id.btn_3).setOnClickListener(this);
        findViewById(R.id.btn_4).setOnClickListener(this);
        findViewById(R.id.btn_5).setOnClickListener(this);
        findViewById(R.id.btn_6).setOnClickListener(this);
        findViewById(R.id.btn_7).setOnClickListener(this);
        findViewById(R.id.btn_8).setOnClickListener(this);
        findViewById(R.id.btn_9).setOnClickListener(this);
        findViewById(R.id.btn_Point).setOnClickListener(this);
        findViewById(R.id.btn_Add).setOnClickListener(this);
        findViewById(R.id.btn_Min).setOnClickListener(this);
        findViewById(R.id.btn_Mul).setOnClickListener(this);
        findViewById(R.id.btn_Div).setOnClickListener(this);
        findViewById(R.id.btn_Lbk).setOnClickListener(this);
        findViewById(R.id.btn_Rbk).setOnClickListener(this);
        findViewById(R.id.btn_Clr).setOnClickListener(this);
        findViewById(R.id.btn_Equ).setOnClickListener(this);
        findViewById(R.id.btn_PoM).setOnClickListener(this);
        Edt = (EditText) findViewById(R.id.edt);
        Tev = (TextView) findViewById(R.id.tev);
        sum = 0;
        n1 = 10;
        n2 = 1;
        str = "";
    }

    public void fnum(int n) {
        t = n;
        str = str + String.valueOf(n);
        sum = sum * n1 + t * n2;
        if (n2 < 1) n2=n2/10;
        Edt.setText(str);
    }

    public void fsign(int n) {
        float m;
        n1=10;
        n2=1;
        switch(n)
        {
            case 11:
                str=str+"+";
                break;
            case 12:
                str=str+"-";
                break;
            case 21:
                str=str+"*";
                break;
            case 22:
                str=str+"/";
                break;
        }
        Edt.setText(str);
        while ( !ss.empty() && ((ss.peek()/10) >= (n/10)) && (ss.peek()!= 31)) {
            m=sn.pop();
            switch (ss.pop())
            {
                case 11:
                    sum=m+sum;
                    break;
                case 12:
                    sum=m-sum;
                    break;
                case 21:
                    sum=m*sum;
                    break;
                case 22:
                    sum=m/sum;
                    break;
            }
        }
        sn.push(sum);
        ss.push(n);
        sum=0;
    }

    public void fequ() {
        int n=1;
        n1=10;
        n2=1;
        float m;
        while ( !ss.empty() && ((ss.peek()/10) >= (n/10)) && (ss.peek()!= 31)) {
            m=sn.pop();
            switch (ss.pop())
            {
                case 11:
                    sum=m+sum;
                    break;
                case 12:
                    sum=m-sum;
                    break;
                case 21:
                    sum=m*sum;
                    break;
                case 22:
                    sum=m/sum;
                    break;
            }
        }
        Tev.setText(String.valueOf(sum));
    }

    public void frbk(){
        int n=2;
        n1=10;
        n2=1;
        float m;
        while ( !ss.empty() && (ss.peek()/10) >= n/10 ) {
            if (ss.peek()== 31){
                ss.pop();
                break;
            }
            m=sn.pop();
            switch (ss.pop())
            {
                case 11:
                    sum=m+sum;
                    break;
                case 12:
                    sum=m-sum;
                    break;
                case 21:
                    sum=m*sum;
                    break;
                case 22:
                    sum=m/sum;
                    break;
            }
        }
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn_0:
                fnum(0);
                break;
            case R.id.btn_1:
                fnum(1);
                break;
            case R.id.btn_2:
                fnum(2);
                break;
            case R.id.btn_3:
                fnum(3);
                break;
            case R.id.btn_4:
                fnum(4);
                break;
            case R.id.btn_5:
                fnum(5);
                break;
            case R.id.btn_6:
                fnum(6);
                break;
            case R.id.btn_7:
                fnum(7);
                break;
            case R.id.btn_8:
                fnum(8);
                break;
            case R.id.btn_9:
                fnum(9);
                break;
            case R.id.btn_Point:
                n1 = 1;
                n2 = n2 / 10;
                str = str + ".";
                Edt.setText(str);
                break;
            case R.id.btn_PoM:
                sum = -sum;
                if (str.indexOf('-') != 0 ) {
                    str = "-" + str;
                } else {
                    str = str.replaceFirst("-", "");
                }
                Edt.setText(str);
                break;
            case R.id.btn_Clr:
                ss.clear();
                sn.clear();
                sum = 0;
                n1 = 10;
                n2 = 1;
                str = "";
                Edt.setText(str);
                Tev.setText("");
                break;
            case R.id.btn_Add:
                fsign(11);
                break;
            case R.id.btn_Min:
                fsign(12);
                break;
            case R.id.btn_Mul:
                fsign(21);
                break;
            case R.id.btn_Div:
                fsign(22);
                break;
            case R.id.btn_Equ:
                fequ();
                break;
            case R.id.btn_Lbk:
                str=str+"(";
                ss.push(31);
                Edt.setText(str);
                break;
            case R.id.btn_Rbk:
                str=str+")";
                Edt.setText(str);
                frbk();
                break;
        }
    }
}
