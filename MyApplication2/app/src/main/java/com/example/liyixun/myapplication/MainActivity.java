package com.example.liyixun.myapplication;


import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioGroup;
import android.widget.TextView;


public class MainActivity extends AppCompatActivity {
    private Button myBtn_one;
    private Button myBtn_two;
    private RadioGroup radioGroup;
    private TextView textView;
    private EditText Edt_acount;
    private EditText Edt_password;
    private Button Btn_login;
    private String str1;
    private String str2;
    public AlertDialog dialog;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout);
        Btn_login = (Button) findViewById(R.id.btn_login);
        Edt_acount = (EditText) findViewById(R.id.edt_account);
        Edt_password = (EditText) findViewById(R.id.edt_password);
        Btn_login.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //str1 = Edt_acount.getText().toString();
                //str2 = Edt_password.getText().toString();
                Edt_password.setText(Edt_acount.getText());
                dialog.show();
            }
        });

        dialog = new AlertDialog.Builder(this).setTitle("Dialog对话框")
                .setMessage("是否确定退出")
                .setIcon(R.mipmap.ic_launcher)
                .setPositiveButton("确定",null)
                .setNegativeButton("取消",null)
                .create();
    }
}