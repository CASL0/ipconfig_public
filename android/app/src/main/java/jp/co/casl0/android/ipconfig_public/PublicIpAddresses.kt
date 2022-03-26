package jp.co.casl0.android.ipconfig_public

import com.orhanobut.logger.Logger
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import java.io.BufferedReader
import java.io.IOException
import java.io.InputStream
import java.io.InputStreamReader
import java.net.HttpURLConnection
import java.net.URL

class PublicIpAddresses(private val _urlList: List<String>) : IpAddresses() {
    constructor(url: String) : this(listOf(url))

    override suspend fun fetchAddressData() {
        withContext(Dispatchers.IO) {
            _urlList.forEach {
                val url = URL(it)
                Logger.d(url)
                try {
                    (url.openConnection() as? HttpURLConnection)?.run {
                        requestMethod = "GET"
                        if (responseCode == HttpURLConnection.HTTP_OK) {
                            Logger.d("http status ok")
                            parseResponse(inputStream).also { body ->
                                if (!_data.contains(body)) {
                                    _data.add(body)
                                }
                            }

                        } else {
                            Logger.e(
                                "HttpUrlConnection failed with status code: $responseCode"
                            )
                        }
                    }
                } catch (e: IOException) {
                    val errorMessage = e.localizedMessage
                    if (errorMessage != null) Logger.d(errorMessage)
                }
            }
        }
    }

    private fun parseResponse(inputStream: InputStream): String {
        val bufferReader = BufferedReader(InputStreamReader(inputStream))
        val responseBody = bufferReader.use { it.readText() }
        bufferReader.close()
        return responseBody
    }
}