cmake_minimum_required (VERSION 3.18)
project (Benchmark)
set(CMAKE_CXX_STANDARD 11)



# 支持gdb调试
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

option(<OPTION_NAME> "<help_text>" [value])


add_subdirectory("3rd-party/gflags")
# add_subdirectory("3rd-party/easyloggingpp")

# 添加共享库搜索路径
if (TARGET_OS STREQUAL "linux")
    # 导入头文件
    include_directories(
        "${CMAKE_SOURCE_DIR}/include"
        "${CMAKE_SOURCE_DIR}/3rd-party/onnxruntime/include/onnxruntime"
        "${CMAKE_SOURCE_DIR}/3rd-party/onnxruntime/include/onnxruntime/core"
        "${CMAKE_SOURCE_DIR}/3rd-party/onnxruntime/include/onnxruntime/core/session"
    )
    link_directories(
        "${CMAKE_SOURCE_DIR}/3rd-party/onnxruntime/build/Linux/RelWithDebInfo"
        )
    set(OrtLib
        onnxruntime)
elseif(TARGET_OS STREQUAL "android")
    add_definitions(-DANDROID_PLATFORM)
    if (TARGET_FRAMEWROK STREQUAL "onnxruntime")
        include_directories(
        "${CMAKE_SOURCE_DIR}/include"
        "${CMAKE_SOURCE_DIR}/include/onnxruntime"
        "${CMAKE_SOURCE_DIR}/include/onnxruntime/core"
        "${CMAKE_SOURCE_DIR}/include/onnxruntime/core/session"
        )
        link_directories(
            "${CMAKE_SOURCE_DIR}/3rd-party/onnxruntime/build/Android/Release"
            )
    elseif(TARGET_FRAMEWROK STREQUAL "paddlelite")
        include_directories(
            "${CMAKE_SOURCE_DIR}/include"
            "${CMAKE_SOURCE_DIR}/include/paddlelite"
        )
        link_directories(
            "${CMAKE_SOURCE_DIR}/3rd-party/Paddle-Lite/build.lite.android.armv8.clang/inference_lite_lib.android.armv8.opencl/cxx/lib"
            )
    elseif(TARGET_FRAMEWROK STREQUAL "ncnn")
        include_directories(
            "${CMAKE_SOURCE_DIR}/include"
            "${CMAKE_SOURCE_DIR}/include/ncnn"
            "${CMAKE_SOURCE_DIR}/include/glslang"
        )
        link_directories(
            "${CMAKE_SOURCE_DIR}/3rd-party/ncnn/build-android-aarch64-vulkan/install/lib"
            )        
    endif()
endif()

add_library(math STATIC ${MATH_SRC})
if (TARGET_FRAMEWROK STREQUAL "onnxruntime")
        set(APP_NAME ort_benchmark)
        # 构建可执行文件
        add_executable(${APP_NAME} 
            "src/onnxruntime_android.cpp"
            "src/easylogging++.cc")
        set(OrtLib
            onnxruntime)
        # 链接共享库
        target_link_libraries(
            ${APP_NAME} 
            ${OrtLib}
            gflags::gflags
            easyloggingpp
        )
elseif(TARGET_FRAMEWROK STREQUAL "paddlelite")
    set(APP_NAME paddlelite_benchmark)
    add_executable(${APP_NAME} "src/paddlelite_android.cpp")
    set(PaddleLiteLib
    # paddle_full_api_shared
    paddle_full_api_shared)
    target_link_libraries(
        ${APP_NAME} 
        ${PaddleLiteLib}
        gflags::gflags
        )
elseif(TARGET_FRAMEWROK STREQUAL "ncnn")
set(APP_NAME ncnn_benchmark)
    add_executable(${APP_NAME} 
    "src/ncnn_android.cpp"
    "src/easylogging++.cc")
    set(ncnnLib
    ncnn)
    target_link_libraries(
        ${APP_NAME}
        ${ncnnLib}
        gflags::gflags
        )
endif()




