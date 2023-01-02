# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.

project("sevenzlzma")

add_library(plzma SHARED IMPORTED)
set_target_properties(plzma PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/${ANDROID_ABI}/libplzma.so)
set(CMAKE_CXX_STANDARD 20)

add_library( # Sets the name of the library.
        sevenzlzma
        SHARED
        sevenzlzma.cpp libplzma.hpp jni_exception.cpp fd_stream.cpp)

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

target_link_libraries( # Specifies the target library.
        sevenzlzma
        ${log-lib} plzma)