package jp.co.casl0.android.ipconfig_public.ui.main

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.Transformations
import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import jp.co.casl0.android.ipconfig_public.PrivateIpAddresses

class PageViewModel : ViewModel() {
    private var _privateIpAddresses = MutableLiveData<List<String>>(listOf(""))
    val privateIpAddresses: LiveData<List<String>>
        get() = _privateIpAddresses

    private var _publicIpAddresses = MutableLiveData<List<String>>(listOf(""))
    val publicIpAddresses: LiveData<List<String>>
        get() = _publicIpAddresses

    fun updateIpAddresses() {
        val privateIp = PrivateIpAddresses().also {
            it.fetchAddressData()
        }
        _privateIpAddresses.value = privateIp.data
    }
}