package com.github.awxkee.sevenzlzma

import androidx.annotation.Keep

@Keep
class SevenLzma {

    fun isPasswordProtected(archive: String): Boolean {
        return !validatePasswordImpl(archive, null)
    }

    fun validatePassword(archive: String, password: String?): Boolean {
        return validatePasswordImpl(archive, password)
    }

    fun extract(archive: String, to: String, password: String?) {
        extractImpl(archive, to, password)
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

    private external fun validatePasswordImpl(from: String, password: String?): Boolean

    private external fun extractImpl(from: String, to: String, password: String?)

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