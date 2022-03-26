package jp.co.casl0.android.ipconfig_public.app_database

import androidx.room.Dao
import androidx.room.Insert
import androidx.room.Delete
import androidx.room.Query
import androidx.room.OnConflictStrategy

@Dao
interface AddressDAO {
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    fun insertAddresses(vararg addresses: Address)

    @Delete
    fun deleteAddresses(vararg addresses: Address)

    @Query("SELECT * FROM addresses")
    fun getAll(): List<Address>

    @Query("SELECT address FROM addresses")
    fun getAllAddresses(): List<String>
}