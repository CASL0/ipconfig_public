package jp.co.casl0.android.ipconfig_public

import android.content.Context
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.orhanobut.logger.Logger

class IpListAdapter(context: Context?, var ipList: List<String>) :
    RecyclerView.Adapter<IpListAdapter.IpListViewHolder>() {
    private val _inflater: LayoutInflater = LayoutInflater.from(context)

    inner class IpListViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        val itemTextView: TextView = itemView.findViewById(R.id.listItemTextView)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): IpListViewHolder {
        Logger.d("onCreateViewHolder")
        val itemView: View = _inflater.inflate(R.layout.list_item, parent, false)
        return IpListViewHolder(itemView)
    }

    override fun onBindViewHolder(holder: IpListViewHolder, position: Int) {
        Logger.d("onBindViewHolder")
        val text = ipList[position]
        holder.itemTextView.text = text
    }

    override fun getItemCount(): Int {
        return ipList.size
    }
}