package jp.co.casl0.android.ipconfig_public

import android.util.Log
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import java.net.NetworkInterface

class PrivateIpAddresses : IpAddresses() {
    override suspend fun fetchAddressData() {
        withContext(Dispatchers.IO) {
            val interfaces = NetworkInterface.getNetworkInterfaces() ?: return@withContext

            for (intf in interfaces) {
                Log.d(PrivateIpAddresses::class.java.simpleName, intf.displayName)
                when {
                    intf.isLoopback -> {
                        //ループバックは無視
                        continue
                    }
                    !intf.isUp -> {
                        //リンクダウンしているインターフェースは無視
                        continue
                    }
                    intf.isVirtual -> {
                        //仮想NICは無視
                        continue
                    }
                }
                for (addr in intf.inetAddresses) {
                    when {
                        addr.isLoopbackAddress -> {
                            //ループバックは無視
                            continue
                        }
                    }

                    val hostAddr = addr.hostAddress
                    if (hostAddr != null) {
                        Log.d(PrivateIpAddresses::class.java.simpleName, hostAddr)
                        _data.add(hostAddr)
                    }
                }
            }
        }

    }
}