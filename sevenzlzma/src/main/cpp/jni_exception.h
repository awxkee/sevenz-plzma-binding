//
// Created by Radzivon Bartoshyk on 01/01/2023.
//

#ifndef AVIF_JNI_EXCEPTION_H
#define AVIF_JNI_EXCEPTION_H

#include <jni.h>

jint throwInvalidFilesPreconditionException(JNIEnv *env, const char * msg);
jint throwCantCreateEncoder(JNIEnv *env);
jint throwEncodingFailedException(JNIEnv *env, const char * msg);
jint throwDecodingFailedException(JNIEnv *env, const char * msg);
jint throwCantCreateDecoder(JNIEnv *env);
jint throwCantExtractFilesException(JNIEnv *env);

#endif //AVIF_JNI_EXCEPTION_H
