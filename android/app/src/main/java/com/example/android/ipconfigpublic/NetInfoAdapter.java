package com.example.android.ipconfigpublic;

import android.content.Context;
import android.util.Pair;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.LinkedList;

public class NetInfoAdapter extends RecyclerView.Adapter<NetInfoAdapter.NetInfoViewHolder> {
    private LinkedList<Pair<String,String>> mNetInfoList;
    private final LayoutInflater mInflater;

    public NetInfoAdapter(Context context, LinkedList<Pair<String,String>> netInfoList) {
        mInflater = LayoutInflater.from(context);
        mNetInfoList = netInfoList;
    }

    @NonNull
    @Override
    public NetInfoViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View itemView = mInflater.inflate(R.layout.netinfo_item, parent, false);
        return new NetInfoViewHolder(itemView);
    }

    @Override
    public void onBindViewHolder(@NonNull NetInfoViewHolder holder, int position) {
        Pair<String,String> netInfo = mNetInfoList.get(position);
        holder.mLabelTextView.setText(netInfo.first);
        holder.mValueTextView.setText(netInfo.second);
    }

    @Override
    public int getItemCount() {
        return mNetInfoList.size();
    }

    public class NetInfoViewHolder extends RecyclerView.ViewHolder {
        private final TextView mLabelTextView;
        private final TextView mValueTextView;

        public NetInfoViewHolder(@NonNull View itemView) {
            super(itemView);
            mLabelTextView = itemView.findViewById(R.id.label);
            mValueTextView = itemView.findViewById(R.id.netinfo);
        }
    }
}
