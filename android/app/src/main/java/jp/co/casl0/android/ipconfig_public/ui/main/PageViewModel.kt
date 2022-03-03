package jp.co.casl0.android.ipconfig_public.ui.main

import androidx.lifecycle.*
import jp.co.casl0.android.ipconfig_public.PrivateIpAddresses
import jp.co.casl0.android.ipconfig_public.PublicIpAddresses
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch

class PageViewModel : ViewModel() {
    private var _privateIpAddresses = MutableLiveData<List<String>>(listOf(""))
    val privateIpAddresses: LiveData<List<String>>
        get() = _privateIpAddresses

    private var _publicIpAddresses = MutableLiveData<List<String>>(listOf(""))
    val publicIpAddresses: LiveData<List<String>>
        get() = _publicIpAddresses

    fun updateIpAddresses() {
        PrivateIpAddresses().also {
            it.fetchAddressData()
            _privateIpAddresses.postValue(it.data)
        }
        viewModelScope.launch(Dispatchers.IO) {
            PublicIpAddresses("https://api64.ipify.org").also {
                it.fetchAddressData()
                _publicIpAddresses.postValue(it.data)
            }
        }
    }
}