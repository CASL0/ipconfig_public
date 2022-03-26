package jp.co.casl0.android.ipconfig_public

import android.app.Application
import androidx.room.Room
import com.orhanobut.logger.Logger
import jp.co.casl0.android.ipconfig_public.app_database.AppDatabase

class IpApplication : Application() {
    companion object {
        lateinit var appDatabase: AppDatabase
    }

    override fun onCreate() {
        super.onCreate()
        appDatabase =
            Room.databaseBuilder(applicationContext, AppDatabase::class.java, "app_database")
                .build()
        Logger.d("app_database build")
    }
}