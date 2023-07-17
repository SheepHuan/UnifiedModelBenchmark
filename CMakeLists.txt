cmake_minimum_required (VERSION 3.18)
project (Benchmark)
set(CMAKE_CXX_STANDARD 11)



# 支持gdb调试
# SET(CMAKE_BUILD_TYPE "Debug")
# SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
# SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

option(BUILD_NCNN "" OFF)
option(BUILD_ONNXRUNTIME "" OFF)
option(BUILD_LINUX_CPU "" OFF)
option(BUILD_ANDROID "" OFF)


add_subdirectory("3rd-party/gflags")
add_subdirectory("3rd-party/glog")

include_directories(
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/include/gflags"
    "${CMAKE_SOURCE_DIR}/inlcude/glog"
    "${CMAKE_SOURCE_DIR}/inlcude/mutils"
)

# framework option
if (BUILD_NCNN)
    message("ncnn")
    set(APP_NAME "ncnn_benchmark")
    include_directories(
        "${CMAKE_SOURCE_DIR}/include/ncnn"
    )
    link_directories(
        "${CMAKE_SOURCE_DIR}/libs/ncnn-20230517-android-vulkan-shared/arm64-v8a/lib"
    )
    set(libs ncnn)
    add_executable(${APP_NAME} 
    "src/ncnn_android.cpp")
elseif (BUILD_ONNXRUNTIME)
    message("ort")
    set(APP_NAME "ort_benchmark")
    include_directories(
        "${CMAKE_SOURCE_DIR}/include/onnxruntime"
        "${CMAKE_SOURCE_DIR}/include/onnxruntime/core"
        "${CMAKE_SOURCE_DIR}/include/onnxruntime/core/session"
    )
    link_directories(
        "/root/workspace/UnifiedModelBenchmark/libs/onnxruntime-v1.14.1-android_shared/libs"
    )
    set(libs onnxruntime)
    add_executable(${APP_NAME} 
    "src/onnxruntime_android.cpp")
endif()

target_link_libraries(
    ${APP_NAME}
    gflags::gflags
    glog::glog
    ${libs}
    )



