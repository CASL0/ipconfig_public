package jp.co.casl0.android.ipconfig_public

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import androidx.recyclerview.widget.DividerItemDecoration
import androidx.recyclerview.widget.LinearLayoutManager
import com.orhanobut.logger.Logger
import jp.co.casl0.android.ipconfig_public.databinding.ActivityLicenseBinding

class LicenseActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val binding = ActivityLicenseBinding.inflate(layoutInflater)
        setContentView(binding.root)

        val adapter = TextListAdapter(this, getLicenses())
        val recyclerView = binding.licenseList
        recyclerView.adapter = adapter
        recyclerView.layoutManager = LinearLayoutManager(this)

        //区切り線
        DividerItemDecoration(
            this,
            DividerItemDecoration.VERTICAL
        ).also { recyclerView.addItemDecoration(it) }

        Logger.d("licenses displayed")
    }

    private fun getLicenses(): List<String> {
        val licenses = mutableListOf<String>()
        resources.openRawResource(R.raw.orhanobut_logger_license).bufferedReader()
            .use { it.readText() }.also { license ->
                licenses.add(license)
            }
        return licenses
    }
}