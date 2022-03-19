package jp.co.casl0.android.ipconfig_public.ui.main

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProvider
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.google.android.material.floatingactionbutton.FloatingActionButton
import com.google.android.material.snackbar.Snackbar
import com.orhanobut.logger.Logger
import jp.co.casl0.android.ipconfig_public.R
import jp.co.casl0.android.ipconfig_public.TextListAdapter
import jp.co.casl0.android.ipconfig_public.databinding.FragmentMainBinding

/**
 * A placeholder fragment containing a simple view.
 */
class PlaceholderFragment : Fragment() {

    private lateinit var pageViewModel: PageViewModel
    private var _binding: FragmentMainBinding? = null

    // This property is only valid between onCreateView and
    // onDestroyView.
    private val binding get() = _binding!!

    private var _tabNumber: Int = 1

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        pageViewModel = ViewModelProvider(this)[PageViewModel::class.java]
        _tabNumber = arguments?.getInt(ARG_SECTION_NUMBER) ?: 1
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        Logger.d("onCreateView")
        _binding = FragmentMainBinding.inflate(inflater, container, false)
        val root = binding.root

        //リストの設定
        val adapter = TextListAdapter(context, listOf(""))
        val recyclerView: RecyclerView = binding.ipList
        recyclerView.adapter = adapter
        recyclerView.layoutManager = LinearLayoutManager(context)
        when (_tabNumber) {
            1 -> {
                pageViewModel.privateIpAddresses.observe(viewLifecycleOwner, Observer {
                    adapter.textList = it
                    recyclerView.adapter?.notifyDataSetChanged()
                })
            }
            2 -> {
                pageViewModel.publicIpAddresses.observe(viewLifecycleOwner, Observer {
                    adapter.textList = it
                    recyclerView.adapter?.notifyDataSetChanged()
                })
            }
        }

        //フローティングアクションボタンの設定
        val fab: FloatingActionButton = binding.fab
        fab.setOnClickListener { view ->
            pageViewModel.updateIpAddresses()
            Snackbar.make(view, getString(R.string.update_message), Snackbar.LENGTH_LONG)
                .setAction("Action", null).show()
        }

        pageViewModel.updateIpAddresses()
        return root
    }

    companion object {
        /**
         * The fragment argument representing the section number for this
         * fragment.
         */
        private const val ARG_SECTION_NUMBER = "section_number"

        /**
         * Returns a new instance of this fragment for the given section
         * number.
         */
        @JvmStatic
        fun newInstance(sectionNumber: Int): PlaceholderFragment {
            return PlaceholderFragment().apply {
                arguments = Bundle().apply {
                    putInt(ARG_SECTION_NUMBER, sectionNumber)
                }
            }
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        Logger.d("onDestroyView")
        _binding = null
    }
}