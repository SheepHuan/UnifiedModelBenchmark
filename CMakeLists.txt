cmake_minimum_required (VERSION 3.18)
project (Benchmark)
set(CMAKE_CXX_STANDARD 17)
set(APP_NAME main)

# 支持gdb调试
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# 导入头文件
include_directories(
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/include/opencv"
    "${CMAKE_SOURCE_DIR}/include/onnxruntime"
)
# 添加共享库搜索路径
if (TARGET_OS STREQUAL "linux")
    link_directories(
        "${CMAKE_SOURCE_DIR}/libs/onnxruntime/linux/amd64"
        "${CMAKE_SOURCE_DIR}/libs/opencv/linux/amd64")
elseif(TARGET_OS STREQUAL "android")
    link_directories(
        "${CMAKE_SOURCE_DIR}/libs/onnxruntime/android/arm64-v8a"
        "${CMAKE_SOURCE_DIR}/libs/opencv/android/arm64-v8a/4.7.0")
endif()
# 声明共享库
set(OpenCVLib
    opencv_dnn
    opencv_videoio
    opencv_imgcodecs
    opencv_photo
    opencv_video
    opencv_core
    opencv_flann
    opencv_gapi
    opencv_ml
    opencv_stitching
    opencv_highgui
    opencv_calib3d
    opencv_imgproc
    opencv_features2d
    opencv_objdetect
)

set(OrtLib
    onnxruntime)

# 构建可执行文件
add_executable(${APP_NAME} "src/onnxruntime_android.cpp")

# 链接共享库
target_link_libraries(
    ${APP_NAME} 
    ${OpenCVLib} 
    ${OrtLib}
)