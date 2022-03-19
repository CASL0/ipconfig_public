package jp.co.casl0.android.ipconfig_public

import android.content.Context
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.orhanobut.logger.Logger

class TextListAdapter(context: Context?, var textList: List<String>) :
    RecyclerView.Adapter<TextListAdapter.TextListViewHolder>() {
    private val _inflater: LayoutInflater = LayoutInflater.from(context)
    private var _fontSize: Float = 20.0f

    constructor(context: Context?, textList: List<String>, fontSize: Float) : this(
        context,
        textList
    ) {
        _fontSize = fontSize
    }

    inner class TextListViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        val itemTextView: TextView = itemView.findViewById(R.id.listItemTextView)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): TextListViewHolder {
        Logger.d("onCreateViewHolder")
        val itemView: View = _inflater.inflate(R.layout.list_item, parent, false)
        return TextListViewHolder(itemView)
    }

    override fun onBindViewHolder(holder: TextListViewHolder, position: Int) {
        Logger.d("onBindViewHolder")
        val text = textList[position]
        holder.itemTextView.text = text
        holder.itemTextView.textSize = _fontSize
    }

    override fun getItemCount(): Int {
        return textList.size
    }
}