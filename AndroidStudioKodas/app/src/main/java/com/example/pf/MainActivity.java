package com.example.pf;

import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.SeekBar;
import android.os.Bundle;
import android.util.Log;
import android.content.Intent;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Base64;
import android.text.TextUtils;
import java.util.Objects;
import android.widget.Toast;
import android.content.Context;

public class MainActivity extends AppCompatActivity {

    EditText editIp, editTimes, username,password;
    Button btnOn, btnOne, login, photoButton;
    TextView textInfo1, textInfo2;
    private SeekBar sBar;
    private TextView tView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        editIp = (EditText)findViewById(R.id.ip);
        btnOn = (Button)findViewById(R.id.bon);
        btnOne = (Button)findViewById(R.id.bonOne);
        textInfo1 = (TextView)findViewById(R.id.info1);
        textInfo2 = (TextView)findViewById(R.id.info2);
        this.sBar = (SeekBar) findViewById(R.id.seekBar );
        this.tView = (TextView) findViewById(R.id.seekBarInterbalNumber);
        photoButton = (Button) findViewById(R.id.photoButton);

        photoButton.setEnabled(false);

        btnOn.setOnClickListener(btnOnOffClickListener);
        btnOne.setOnClickListener(btnOneClickListener);

        tView.setText(sBar.getProgress() + "/" + sBar.getMax());
        sBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            int pval = 0;
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                pval = progress;
            }
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                //write custom code to on start progress
            }
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                tView.setText(pval + "/" + seekBar.getMax());
            }
        });

    }

    View.OnClickListener btnOnOffClickListener = new View.OnClickListener(){
        @Override
        public void onClick(View v) {

            btnOn.setEnabled(false);
            btnOne.setEnabled(false);
            int times = sBar.getProgress();

            if (times == 0) {
                String serverIP = editIp.getText().toString() + "/?module=spin&n=1";
                TaskEsp taskEsp = new TaskEsp(serverIP);
                taskEsp.execute();
            } else {
                String serverIP = editIp.getText().toString() + "/?module=spin&n=" + times;
                TaskEsp taskEsp = new TaskEsp(serverIP);
                taskEsp.execute();
                }
            }
    };

    View.OnClickListener btnOneClickListener = new View.OnClickListener(){
        @Override
        public void onClick(View v) {

            btnOn.setEnabled(false);
            btnOne.setEnabled(false);
            String serverIP = editIp.getText().toString() + "/?module=spin&n=1";
            TaskEsp taskEsp = new TaskEsp(serverIP);
            taskEsp.execute();

        }
    };

    private class TaskEsp extends AsyncTask<Void, Void, String> {

        String server;

        TaskEsp(String server){
            this.server = server;
        }

        @Override
        protected String doInBackground(Void... params) {

            final String p = "http://"+server;

            runOnUiThread(new Runnable(){
                @Override
                public void run() {
                    textInfo1.setText(p);
                }
            });

            String serverResponse = "";

            try {
                HttpURLConnection httpURLConnection = (HttpURLConnection)(new URL(p).openConnection());

                String userCredentials = "iotpetcare:iotpetcare";
                String basicAuth = "Basic " + new String(Base64.getEncoder().encode(userCredentials.getBytes()));

                httpURLConnection.setRequestProperty ("Authorization", basicAuth);
                httpURLConnection.setRequestMethod("GET");

                textInfo2.setText("Bandoma prisijungti");
                try
                {
                    Thread.sleep(1000);
                }
                catch(InterruptedException ex)
                {
                    Thread.currentThread().interrupt();
                }
                if(httpURLConnection.getResponseCode() == HttpURLConnection.HTTP_OK) {
                    InputStream inputStream = null;
                    inputStream = httpURLConnection.getInputStream();
                    BufferedReader bufferedReader =
                            new BufferedReader(new InputStreamReader(inputStream));
                    serverResponse = bufferedReader.readLine();

                    inputStream.close();
                }
            } catch (MalformedURLException e) {
                e.printStackTrace();
                serverResponse = e.getMessage();
            } catch (IOException e) {
                e.printStackTrace();
                serverResponse = e.getMessage();
            }

            return serverResponse;
        }

        @Override
        protected void onPostExecute(String s) {
            textInfo2.setText(s);
            btnOn.setEnabled(true);
            btnOne.setEnabled(true);
        }
    }
}
