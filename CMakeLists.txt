cmake_minimum_required (VERSION 3.18)
project (Benchmark)
set(CMAKE_CXX_STANDARD 11)
set(APP_NAME main)


# 支持gdb调试
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")


# 添加共享库搜索路径
if (TARGET_OS STREQUAL "linux")
    # 导入头文件
    include_directories(
        "${CMAKE_SOURCE_DIR}/include"
        # "${CMAKE_SOURCE_DIR}/3rd-party/opencv/build/install/include/opencv4"
        # "${CMAKE_SOURCE_DIR}/3rd-party/opencv/build/install/include/opencv4/opencv2"
        "${CMAKE_SOURCE_DIR}/3rd-party/onnxruntime/include/onnxruntime"
        "${CMAKE_SOURCE_DIR}/3rd-party/onnxruntime/include/onnxruntime/core"
        "${CMAKE_SOURCE_DIR}/3rd-party/onnxruntime/include/onnxruntime/core/session"
    )
    link_directories(
        "${CMAKE_SOURCE_DIR}/3rd-party/onnxruntime/build/Linux/RelWithDebInfo"
        # "${CMAKE_SOURCE_DIR}/3rd-party/opencv/install/lib"
        )
elseif(TARGET_OS STREQUAL "android")
    add_definitions(-DANDROID_PLATFORM)
    include_directories(
    "${CMAKE_SOURCE_DIR}/include"
    # "${CMAKE_SOURCE_DIR}/3rd-party/opencv/install/sdk/native/jni/include"
    # "${CMAKE_SOURCE_DIR}/3rd-party/opencv/install/sdk/native/jni/include/opencv2"
    "${CMAKE_SOURCE_DIR}/include/onnxruntime"
    "${CMAKE_SOURCE_DIR}/include/onnxruntime/core"
    "${CMAKE_SOURCE_DIR}/include/onnxruntime/core/session"
    )
    link_directories(
        "${CMAKE_SOURCE_DIR}/3rd-party/onnxruntime/build/Android/Debug"
        # "${CMAKE_SOURCE_DIR}/3rd-party/opencv/install/sdk/native/libs/arm64-v8a"
        # "${CMAKE_SOURCE_DIR}/libs/gflags/android/arm64-v8a"
        )
endif()



# # 声明共享库
# set(OpenCVLib
#     # opencv_dnn
#     opencv_videoio
#     opencv_imgcodecs
#     opencv_video
#     opencv_core
#     opencv_gapi
#     opencv_highgui
#     opencv_imgproc
# )

set(OrtLib
    onnxruntime)

# set(gflagsLib
#     gflags_nothreads)

# add_subdirectory("3rd-party/opencv")
add_subdirectory("3rd-party/gflags")

# 构建可执行文件
add_executable(${APP_NAME} "src/onnxruntime_android.cpp")

# 链接共享库
target_link_libraries(
    ${APP_NAME} 
    # ${OpenCVLib} 
    ${OrtLib}
    gflags::gflags
)