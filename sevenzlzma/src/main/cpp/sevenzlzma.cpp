#include <jni.h>
#include <string>
#include "libplzma.hpp"
#include "jni_exception.h"
#include <exception>
#include <vector>
#include <filesystem>
#include <android/log.h>

using namespace plzma;

extern "C"
JNIEXPORT void JNICALL
Java_com_github_awxkee_sevenzlzma_SevenLzma_compressImpl(JNIEnv *env, jobject thiz, jstring to,
                                                         jobjectArray jFiles, jstring jpassword,
                                                         jint jCompressionLevel) {
    auto totalFiles = env->GetArrayLength(jFiles);
    if (totalFiles < 0) {
        throwInvalidFilesPreconditionException(env, "Invalid files count. Must be > 0");
        return;
    }
    auto javaPath = env->GetStringUTFChars(to, nullptr);
    std::string archivePath(javaPath);
    env->ReleaseStringUTFChars(to, javaPath);
    std::vector<std::string> files;
    for (int i = 0; i < totalFiles; ++i) {
        auto fileString = reinterpret_cast<jstring>(env->GetObjectArrayElement(jFiles, i));
        if (!fileString) {
            throwInvalidFilesPreconditionException(env, "Files path must not be null");
            return;
        }
        auto javaNewPath = env->GetStringUTFChars(fileString, nullptr);
        std::string newPath(javaNewPath);
        env->ReleaseStringUTFChars(fileString, javaNewPath);
        files.push_back(newPath);
    }
    bool usePassword = false;
    std::string password;
    if (jpassword) {
        auto javaPassword = env->GetStringUTFChars(jpassword, nullptr);
        if (strlen(javaPassword) > 0) {
            usePassword = true;
            password = javaPassword;
        }
        env->ReleaseStringUTFChars(jpassword, javaPassword);
    }

    try {
        Path sourceArchive(archivePath.c_str());
        auto archiveOutStream = makeSharedOutStream(sourceArchive);
        auto encoder = makeSharedEncoder(archiveOutStream, plzma_file_type_7z,
                                         plzma_method_LZMA2);
        if (usePassword && password.length() > 0) {
            encoder->setPassword(password.c_str());
            encoder->setShouldEncryptContent(true);
        }
        encoder->setCompressionLevel(jCompressionLevel);
        for (auto &file: files) {
            encoder->add(Path(file.c_str()));
        }
        if (!encoder->open()) {
            throwCantCreateEncoder(env);
            return;
        }
        if (!encoder->compress()) {
            if (std::filesystem::exists(archivePath)) {
                std::filesystem::remove_all(archivePath);
            }
            throwEncodingFailedException(env, "Compression was failed");
        }

    } catch (const Exception &exception) {
        throwEncodingFailedException(env, exception.what());
    } catch (const std::exception &exception) {
        throwEncodingFailedException(env, exception.what());
    } catch (...) {
        throwEncodingFailedException(env, "Unknown exception");
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_github_awxkee_sevenzlzma_SevenLzma_extractImpl(JNIEnv *env, jobject thiz, jstring from,
                                                        jstring to,
                                                        jstring jpassword) {
    try {
        auto javaFromPath = env->GetStringUTFChars(from, nullptr);
        std::string sourcePath(javaFromPath);
        env->ReleaseStringUTFChars(from, javaFromPath);
        auto sourceStream = makeSharedInStream(Path(sourcePath.c_str()));
        auto decoder = makeSharedDecoder(sourceStream, plzma_file_type_7z);
        bool usePassword = false;
        std::string password;
        if (jpassword) {
            auto javaPassword = env->GetStringUTFChars(jpassword, nullptr);
            if (strlen(javaPassword) > 0) {
                usePassword = true;
                password = javaPassword;
            }
            env->ReleaseStringUTFChars(jpassword, javaPassword);
        }
        if (usePassword && password.length() > 0) {
            decoder->setPassword(password.c_str());
        }
        if (!decoder->open()) {
            throwCantCreateDecoder(env);
            return;
        }
        auto allArchiveItems = decoder->items();
        auto javaOutPath = env->GetStringUTFChars(to, nullptr);
        std::string outPath(javaOutPath);
        env->ReleaseStringUTFChars(to, javaOutPath);
        if (!decoder->extract(Path(outPath.c_str()))) {
            throwCantExtractFilesException(env);
        }
    } catch (const Exception &exception) {
        throwDecodingFailedException(env, exception.what());
    } catch (const std::exception &exception) {
        throwDecodingFailedException(env, exception.what());
    } catch (...) {
        throwDecodingFailedException(env, "Unknown exception");
    }
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_github_awxkee_sevenzlzma_SevenLzma_validatePasswordImpl(JNIEnv *env, jobject thiz,
                                                                 jstring from, jstring jpassword) {
    try {
        auto javaFromPath = env->GetStringUTFChars(from, nullptr);
        std::string sourcePath(javaFromPath);
        env->ReleaseStringUTFChars(from, javaFromPath);
        auto sourceStream = makeSharedInStream(Path(sourcePath.c_str()));
        auto decoder = makeSharedDecoder(sourceStream, plzma_file_type_7z);
        bool usePassword = false;
        std::string password;
        if (jpassword) {
            auto javaPassword = env->GetStringUTFChars(jpassword, nullptr);
            if (strlen(javaPassword) > 0) {
                usePassword = true;
                password = javaPassword;
            }
            env->ReleaseStringUTFChars(jpassword, javaPassword);
        }
        if (usePassword && password.length() > 0) {
            decoder->setPassword(password.c_str());
        }
        try {
            if (!decoder->open()) {
                throwCantCreateDecoder(env);
                return false;
            }
        } catch (const Exception &exception) {
            __android_log_write(ANDROID_LOG_ERROR, "sevenzlzma", exception.what());
            return false;
        }
        try {
            if (!decoder->test()) {
                return false;
            }
        } catch (const Exception &exception) {
            __android_log_write(ANDROID_LOG_ERROR, "sevenzlzma", exception.what());
            return false;
        }
        return true;
    } catch (const Exception &exception) {
        throwDecodingFailedException(env, exception.what());
        return false;
    } catch (const std::exception &exception) {
        throwDecodingFailedException(env, exception.what());
        return false;
    } catch (...) {
        throwDecodingFailedException(env, "Unknown exception");
        return false;
    }
}