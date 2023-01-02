package com.github.awxkee.sevenzlzma

import android.os.ParcelFileDescriptor
import androidx.annotation.Keep

@Keep
class SevenLzma {

    fun isPasswordProtected(archive: ParcelFileDescriptor): Boolean {
        return !validatePasswordImpl(archive.fd, null)
    }

    fun validatePassword(archive: ParcelFileDescriptor, password: String?): Boolean {
        return validatePasswordImpl(archive.fd, password)
    }

    fun extract(pfd: ParcelFileDescriptor, to: String, password: String?) {
        extractFdImpl(pfd.fd, to, password)
    }

    fun compress(
        to: String, files: List<String>, password: String? = null,
        @androidx.annotation.IntRange(
            from = 0,
            to = 9
        ) compressionLevel: Int = 7
    ) {
        if (files.isEmpty()) {
            throw InvalidFilesPreconditionException("Invalid files count. Must be > 0")
        }
        if (compressionLevel < 0 || compressionLevel > 9) {
            throw Exception("Compression level must be 0..9")
        }
        compressImpl(to, files.toTypedArray(), password, compressionLevel)
    }

    private external fun validatePasswordImpl(fd: Int, password: String?): Boolean

    private external fun extractFdImpl(fd: Int, to: String, password: String?)

    private external fun compressImpl(
        to: String,
        files: Array<String>,
        password: String?,
        @androidx.annotation.IntRange(
            from = 0,
            to = 9
        ) compressionLevel: Int
    )

    companion object {
        init {
            System.loadLibrary("sevenzlzma")
        }
    }
}