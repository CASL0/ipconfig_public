package jp.co.casl0.android.ipconfig_public.ui.main

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.orhanobut.logger.Logger
import jp.co.casl0.android.ipconfig_public.IpApplication
import jp.co.casl0.android.ipconfig_public.PrivateIpAddresses
import jp.co.casl0.android.ipconfig_public.PublicIpAddresses
import jp.co.casl0.android.ipconfig_public.app_database.Address
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import java.text.SimpleDateFormat
import java.util.*

class PageViewModel : ViewModel() {
    private var _privateIpAddresses = MutableLiveData<List<String>>(listOf(""))
    val privateIpAddresses: LiveData<List<String>>
        get() = _privateIpAddresses

    private var _publicIpAddresses = MutableLiveData<List<String>>(listOf(""))
    val publicIpAddresses: LiveData<List<String>>
        get() = _publicIpAddresses

    private val _recentIpAddresses = MutableLiveData(listOf(""))
    val recentIpAddresses: LiveData<List<String>>
        get() = _recentIpAddresses

    fun updateIpAddresses() {
        Logger.d("updateIpAddresses")
        viewModelScope.launch {
            PrivateIpAddresses().also {
                it.fetchAddressData()
                _privateIpAddresses.postValue(it.data)
            }
        }
        viewModelScope.launch {
            //IPv4とIPv6のパブリックIPを取得
            PublicIpAddresses(listOf("https://api.ipify.org", "https://api64.ipify.org")).also {
                it.fetchAddressData()
                _publicIpAddresses.postValue(it.data)
                insertAddressToDatabase(it.data)
            }
        }
        viewModelScope.launch(Dispatchers.IO) {
            _recentIpAddresses.postValue(IpApplication.appDatabase.addressDao().getAllAddresses())
        }
    }

    private suspend fun insertAddressToDatabase(addresses: List<String>) {
        withContext(Dispatchers.IO) {
            val now = Calendar.getInstance().toFormatString("yyyy-MM-dd")
            Logger.d("insert to db: $now")
            addresses.forEach { address ->
                IpApplication.appDatabase.addressDao()
                    .insertAddresses(Address(address, now))
            }
        }
    }
}

fun Calendar.toFormatString(format: String): String {
    SimpleDateFormat(format).run {
        return format(time)
    }
}