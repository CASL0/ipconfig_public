package jp.co.casl0.android.ipconfig_public

import android.util.Log
import java.io.BufferedReader
import java.io.InputStream
import java.io.InputStreamReader
import java.net.HttpURLConnection
import java.net.URL

class PublicIpAddresses(private val _url: String) : IpAddresses() {
    override fun fetchAddressData() {
        val url = URL(_url)
        (url.openConnection() as? HttpURLConnection)?.run {
            requestMethod = "GET"
            if (responseCode == HttpURLConnection.HTTP_OK) {
                _data.add(parseResponse(inputStream))
            } else {
                Log.e(
                    PublicIpAddresses::class.java.simpleName,
                    "HttpUrlConnection failed with status code: $responseCode"
                )
            }
        }
    }

    fun parseResponse(inputStream: InputStream): String {
        val bufferReader = BufferedReader(InputStreamReader(inputStream))
        val responseBody = bufferReader.use { it.readText() }
        bufferReader.close()
        return responseBody
    }
}