package jp.co.casl0.android.ipconfig_public

abstract class IpAddresses {
    protected val _data = mutableListOf<String>()
    val data: List<String>
        get() = _data

    abstract fun fetchAddressData()
}