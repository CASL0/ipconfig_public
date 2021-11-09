package com.example.android.ipconfigpublic;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.util.Pair;

import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Collections;
import java.util.Enumeration;
import java.util.LinkedList;
import java.util.List;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = MainActivity.class.getSimpleName();
    private RecyclerView mNetInfoRecycleView;
    private NetInfoAdapter mAdapter;
    private LinkedList<Pair<String,String>> mNetInfoList = new LinkedList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mNetInfoRecycleView = findViewById(R.id.netinfo_list);
        fetchPrivateIpInfo(mNetInfoList);
        mAdapter = new NetInfoAdapter(this, mNetInfoList);
        mNetInfoRecycleView.setAdapter(mAdapter);
        mNetInfoRecycleView.setLayoutManager(new LinearLayoutManager(this));
        new FetchPublicInfo(mNetInfoList, mNetInfoRecycleView).execute(getString(R.string.url_public_v4), getString(R.string.url_public_v6));
    }

    private void fetchPrivateIpInfo(LinkedList<Pair<String,String>> netInfoList) {
        LinkedList<String> privateAddresses = getPrivateIpAddress();
        String val = TextUtils.join("\n", privateAddresses);
        Pair<String,String> privateIp = new Pair<>(getString(R.string.group_private_info), val);
        netInfoList.addLast(privateIp);
    }

    private LinkedList<String> getPrivateIpAddress() {
        List<NetworkInterface> interfaces;
        try {
            interfaces = Collections.list(NetworkInterface.getNetworkInterfaces());
        } catch (SocketException e) {
            e.printStackTrace();
            return null;
        }

        LinkedList<String> privateAddresses = new LinkedList<>();
        for (NetworkInterface intf : interfaces) {
            Log.d(TAG, intf.getDisplayName());
            try {
                if(intf.isLoopback()) {
                    Log.d(TAG, "interface loop back");
                    continue;
                }else if(!intf.isUp()) {
                    Log.d(TAG, "interface not up");
                    continue;
                }else if(intf.isVirtual()) {
                    Log.d(TAG, "interface virtual");
                    continue;
                }
            } catch (SocketException e) {
                e.printStackTrace();
                continue;
            }
            List<InetAddress> addrs = Collections.list(intf.getInetAddresses());
            for (InetAddress addr : addrs) {
                if(addr.isLoopbackAddress()) {
                    continue;
                }
                Log.d(TAG, addr.getHostAddress());
                privateAddresses.addLast(addr.getHostAddress());
            }
        }
        return privateAddresses;
    }
}