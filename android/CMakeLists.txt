project(ReactNativeMsgPack)
cmake_minimum_required(VERSION 3.9.0)

set (PACKAGE_NAME "react-native-msgpack")
set (BUILD_DIR ${CMAKE_SOURCE_DIR}/build)
set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 17)

include_directories(
        ../cpp
        ../../react-native/React
        ../../react-native/React/Base
        ../../react-native/ReactCommon/jsi
)

add_library(cpp
            SHARED
            cpp-adapter.cpp
            ../cpp/react-native-msgpack.hpp
            ../cpp/cmp.c
            ../cpp/cmp.h
            ../cpp/msgpack.hpp
)

set_target_properties(
        cpp PROPERTIES
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF
        POSITION_INDEPENDENT_CODE ON
)


find_package(ReactAndroid REQUIRED CONFIG)

target_link_libraries(
        cpp
        ReactAndroid::jsi
        android
)
