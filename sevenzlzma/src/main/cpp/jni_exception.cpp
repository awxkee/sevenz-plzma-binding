//
// Created by Radzivon Bartoshyk on 01/01/2023.
//

#include "jni_exception.h"

jint throwInvalidFilesPreconditionException(JNIEnv *env, const char * msg) {
    jclass exClass;
    exClass = env->FindClass("com/github/awxkee/sevenzlzma/InvalidFilesPreconditionException");
    return env->ThrowNew(exClass, msg);
}

jint throwEncodingFailedException(JNIEnv *env, const char * msg) {
    jclass exClass;
    exClass = env->FindClass("com/github/awxkee/sevenzlzma/EncodingFailedException");
    return env->ThrowNew(exClass, msg);
}

jint throwDecodingFailedException(JNIEnv *env, const char * msg) {
    jclass exClass;
    exClass = env->FindClass("com/github/awxkee/sevenzlzma/DecodingException");
    return env->ThrowNew(exClass, msg);
}

jint throwCantCreateEncoder(JNIEnv *env) {
    jclass exClass;
    exClass = env->FindClass("com/github/awxkee/sevenzlzma/Create7zEncoderException");
    return env->ThrowNew(exClass, "");
}

jint throwCantCreateDecoder(JNIEnv *env) {
    jclass exClass;
    exClass = env->FindClass("com/github/awxkee/sevenzlzma/Create7zDecoderException");
    return env->ThrowNew(exClass, "");
}

jint throwCantExtractFilesException(JNIEnv *env) {
    jclass exClass;
    exClass = env->FindClass("com/github/awxkee/sevenzlzma/CantExtract7zFilesException");
    return env->ThrowNew(exClass, "");
}