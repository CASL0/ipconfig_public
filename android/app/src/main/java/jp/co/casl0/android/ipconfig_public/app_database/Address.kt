package jp.co.casl0.android.ipconfig_public.app_database

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.PrimaryKey

@Entity(tableName = "addresses")
data class Address(
    @PrimaryKey val address: String,
    @ColumnInfo(name = "last_use") val lastUse: String
)