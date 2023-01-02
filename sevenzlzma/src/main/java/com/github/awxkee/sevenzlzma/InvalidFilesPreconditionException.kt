package com.github.awxkee.sevenzlzma

import androidx.annotation.Keep

@Keep
class InvalidFilesPreconditionException(message: String?): Exception(message) {
}