cmake_minimum_required (VERSION 3.18)
project (Benchmark)
set(CMAKE_CXX_STANDARD 11)
set(APP_NAME aiot_benchmark)


# 支持gdb调试
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

option(<OPTION_NAME> "<help_text>" [value])


add_subdirectory("3rd-party/gflags")

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
            "${CMAKE_SOURCE_DIR}/3rd-party/onnxruntime/build/Android/Debug"
            )
    elseif(TARGET_FRAMEWROK STREQUAL "paddlelite")
        include_directories(
            "${CMAKE_SOURCE_DIR}/include"
            "${CMAKE_SOURCE_DIR}/include/paddlelite"
        )
        link_directories(
            "${CMAKE_SOURCE_DIR}/3rd-party/Paddle-Lite/build.lite.android.armv8.clang/inference_lite_lib.android.armv8.opencl/cxx/lib"
            )
        
    endif()
endif()


if (TARGET_FRAMEWROK STREQUAL "onnxruntime")
       
        # 构建可执行文件
        add_executable(${APP_NAME} "src/onnxruntime_android.cpp")
        set(OrtLib
            onnxruntime)
        # 链接共享库
        target_link_libraries(
            ${APP_NAME} 
            ${OrtLib}
            gflags::gflags
        )
    elseif(TARGET_FRAMEWROK STREQUAL "paddlelite")
        add_executable(${APP_NAME} "src/paddlelite_android.cpp")
        set(PaddleLiteLib
            paddle_light_api_shared)
        target_link_libraries(
            ${APP_NAME} 
            ${PaddleLiteLib}
            gflags::gflags
            )
    endif()




