package com.example.android.ipconfigpublic;

import android.os.AsyncTask;
import android.text.TextUtils;
import android.util.Log;
import android.util.Pair;

import androidx.recyclerview.widget.RecyclerView;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.lang.ref.WeakReference;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.net.URL;
import java.util.LinkedList;

public class FetchPublicInfo extends AsyncTask<String, Void, LinkedList<String>> {
    private static final String TAG = FetchPublicInfo.class.getSimpleName();
    private WeakReference<RecyclerView> mNetInfoRecycleView;
    private WeakReference<LinkedList<Pair<String,String>>> mNetInfoList;

    public FetchPublicInfo(LinkedList<Pair<String,String>> netInfoList, RecyclerView netInfoRecycleView) {
        mNetInfoList = new WeakReference<>(netInfoList);
        mNetInfoRecycleView = new WeakReference<>(netInfoRecycleView);
    }

    @Override
    protected LinkedList<String> doInBackground(String... strUrls) {
        LinkedList<String> publicIpAddresses = new LinkedList<>();
        for(String strUrl : strUrls) {
            Log.d(TAG, strUrl);
            URL url = null;
            try {
                url = new URL(strUrl);
            } catch (MalformedURLException e) {
                e.printStackTrace();
                continue;
            }

            HttpURLConnection urlConnection = null;
            try {
                urlConnection = (HttpURLConnection) url.openConnection();
                urlConnection.setRequestMethod("GET");
                urlConnection.connect();
            } catch (ProtocolException e) {
                e.printStackTrace();
                continue;
            } catch (IOException e){
                e.printStackTrace();
                continue;
            }

            InputStream inputStream = null;
            try {
                inputStream = urlConnection.getInputStream();
            } catch (IOException e) {
                e.printStackTrace();
            }

            BufferedReader reader = new BufferedReader(new InputStreamReader(inputStream));
            StringBuilder builder = new StringBuilder();
            String line;
            try {
                while ((line = reader.readLine()) != null) {
                    builder.append(line);
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
            if(!builder.toString().isEmpty()) {
                Log.d(TAG, builder.toString());
                publicIpAddresses.addLast(builder.toString());
            }
        }
        return publicIpAddresses;
    }

    @Override
    protected void onProgressUpdate(Void... values) {
        super.onProgressUpdate(values);
    }

    @Override
    protected void onPostExecute(LinkedList<String> publicIpAddresses) {
        super.onPostExecute(publicIpAddresses);
        if(publicIpAddresses == null) {
            Log.d(TAG, "publicIpAddresses is null");
            return;
        }

        int netInfoListSize = mNetInfoList.get().size();
        String val = TextUtils.join("\n", publicIpAddresses);
        Pair<String,String> privateIp = new Pair<>("Public IP", val);
        mNetInfoList.get().addLast(privateIp);
        mNetInfoRecycleView.get().getAdapter().notifyItemInserted(netInfoListSize);
    }
}
