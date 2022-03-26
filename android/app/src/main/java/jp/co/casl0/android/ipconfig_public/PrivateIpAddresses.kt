package jp.co.casl0.android.ipconfig_public

import com.orhanobut.logger.Logger
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import java.net.NetworkInterface
import java.net.SocketException

class PrivateIpAddresses : IpAddresses() {
    override suspend fun fetchAddressData() {
        withContext(Dispatchers.IO) {
            try {
                val interfaces = NetworkInterface.getNetworkInterfaces() ?: return@withContext

                for (intf in interfaces) {
                    Logger.d(intf.displayName)
                    when {
                        intf.isLoopback -> {
                            //ループバックは無視
                            Logger.d("interface is loopback")
                            continue
                        }
                        !intf.isUp -> {
                            //リンクダウンしているインターフェースは無視
                            Logger.d("interface is down")
                            continue
                        }
                        intf.isVirtual -> {
                            //仮想NICは無視
                            Logger.d("interface is virtual")
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
                            Logger.d(hostAddr)
                            _data.add(hostAddr)
                        }
                    }
                }
            } catch (e: SocketException) {
                val errorMessage = e.localizedMessage
                if (errorMessage != null) Logger.d(errorMessage)
            }

        }

    }
}