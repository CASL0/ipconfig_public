package jp.co.casl0.android.ipconfig_public.app_database

import androidx.room.Database
import androidx.room.RoomDatabase

@Database(entities = [Address::class], version = 1)
abstract class AppDatabase : RoomDatabase() {
    abstract fun addressDao(): AddressDAO
}