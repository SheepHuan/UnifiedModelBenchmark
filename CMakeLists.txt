cmake_minimum_required(VERSION 3.18)
project(Benchmark)
set(CMAKE_CXX_STANDARD 14)

# 支持gdb调试
# SET(CMAKE_BUILD_TYPE "Debug")
# SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
# SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
option(BUILD_ONNXRUNTIME "" OFF)
option(BUILD_NCNN "" OFF)
option(BUILD_MNN "" OFF)

option(BUILD_LINUX_CPU "" OFF)
option(BUILD_ANDROID "" OFF)
option(BUILD_HW_NPU "" OFF)

# FOR MNN
option(BUILD_TEST "" OFF)

add_subdirectory("3rd-party/gflags")
add_subdirectory("3rd-party/glog")

include_directories(
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/include/gflags"
    "${CMAKE_SOURCE_DIR}/inlcude/glog"
    "${CMAKE_SOURCE_DIR}/inlcude/mutils"
)

file(GLOB PUBLIC_CPP_SOURCES "src/mutils/*.cpp")

if(BUILD_TEST)
    set(APP_NAME "test")
    set(libs "")
    add_executable(${APP_NAME}
        "src/debug.cpp")
else()
    # framework option
    if(BUILD_MNN)
        message("ncnn")
        set(APP_NAME "mnn_benchmark")

        if(BUILD_LINUX_CPU)
            include_directories(
                "${CMAKE_SOURCE_DIR}/release/mnn_2.6.0_linux_x64_cpu/include"
                "${CMAKE_SOURCE_DIR}/release/mnn_2.6.0_linux_x64_cpu/include/MNN"
            )
            link_directories(
                "${CMAKE_SOURCE_DIR}/release/mnn_2.6.0_linux_x64_cpu/lib"
            )
            set(libs
                MNN
                MNN_Express)
        elseif(BUILD_ANDROID)
            include_directories(
                "${CMAKE_SOURCE_DIR}/release/mnn_2.6.0-android_armv8_cpu_opencl_vulkan_npu/include"
                "${CMAKE_SOURCE_DIR}/release/mnn_2.6.0-android_armv8_cpu_opencl_vulkan_npu/include/MNN"
            )
            link_directories(
                "${CMAKE_SOURCE_DIR}/release/mnn_2.6.0-android_armv8_cpu_opencl_vulkan_npu/arm64-v8a"
            )

            if(BUILD_HW_NPU)
                set(libs
                    MNN
                    MNN_CL
                    MNN_Vulkan
                    MNN_Express
                    plugin_matmul
                    hiai
                    hiai_ir
                    hiai_ir_build
                    MNN_NPU)
            else()
                set(libs
                    MNN_CL
                    MNN_Vulkan
                    MNN
                    MNN_Express)
            endif()
        endif()

        add_executable(${APP_NAME}
            "src/benchmark/mnn_benchmark.cpp"
            ${PUBLIC_CPP_SOURCES})

    elseif(BUILD_NCNN)
        message("ncnn")
        set(APP_NAME "ncnn_benchmark")

        if(BUILD_LINUX_CPU)
            include_directories(
                "${CMAKE_SOURCE_DIR}/release/ncnn-20230517-android-vulkan-shared/arm64-v8a/include"
                "${CMAKE_SOURCE_DIR}/release/ncnn-20230517-ubuntu-2204-shared/include/ncnn"
            )
            link_directories(
                "${CMAKE_SOURCE_DIR}/release/ncnn-20230517-ubuntu-2204-shared/lib"
            )
        elseif(BUILD_ANDROID)
            include_directories(
                "${CMAKE_SOURCE_DIR}/release/ncnn-20230517-android-vulkan-shared/arm64-v8a/include"
                "${CMAKE_SOURCE_DIR}/release/ncnn-20230517-android-vulkan-shared/arm64-v8a/include/ncnn"
            )
            link_directories(
                "${CMAKE_SOURCE_DIR}/release/ncnn-20230517-android-vulkan-shared/arm64-v8a/lib"
            )
        endif()

        set(libs ncnn)
        add_executable(${APP_NAME}
            "src/benchmark/ncnn_benchmark.cpp"
            ${PUBLIC_CPP_SOURCES})
    elseif(BUILD_ONNXRUNTIME)
        message("ort")
        set(APP_NAME "ort_benchmark")

        if(BUILD_LINUX_CPU)
            include_directories(
                "${CMAKE_SOURCE_DIR}/release/onnxrunrime-v1.15.1-linux-shared/include/onnxruntime"
                "${CMAKE_SOURCE_DIR}/release/onnxrunrime-v1.15.1-linux-shared/include/onnxruntime/core"
                "${CMAKE_SOURCE_DIR}/release/onnxrunrime-v1.15.1-linux-shared/include/onnxruntime/core/session"
            )
            link_directories(
                "${CMAKE_SOURCE_DIR}/release/onnxrunrime-v1.15.1-linux-shared/lib"
            )
        elseif(BUILD_ANDROID)
            include_directories(
                "${CMAKE_SOURCE_DIR}/release/onnxruntime-v1.15.1-android-shared/include/onnxruntime"
                "${CMAKE_SOURCE_DIR}/release/onnxruntime-v1.15.1-android-shared/include/onnxruntime/core"
                "${CMAKE_SOURCE_DIR}/release/onnxruntime-v1.15.1-android-shared/include/onnxruntime/core/session"
            )
            link_directories(
                "${CMAKE_SOURCE_DIR}/release/onnxruntime-v1.15.1-android-shared/lib"
            )
        endif()

        set(libs onnxruntime)
        add_executable(${APP_NAME}
            "src/benchmark/onnxruntime_benchmark.cpp"
            ${PUBLIC_CPP_SOURCES})
    endif()
endif()

target_link_libraries(
    ${APP_NAME}
    gflags::gflags
    glog::glog
    ${libs}
)
