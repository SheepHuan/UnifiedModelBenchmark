# Model Benchmark

## 1 安装

### library
1. ncnn 20230517
2. onnxruntime v1.15.1

### 编译benchmark
准备好依赖后运行一下指令即可。准备依赖的[文档](docs/builder/README.md)
```bash
export ANDROID_NDK="/root/android_sdk/ndk/25.0.8775105"
export PATH=$PATH:/root/android_sdk/cmake/3.22.1/bin

# linux 交叉编译 android 可执行文件，要求先编译完依赖库onnxruntime.so paddlelite.so ncnn.so
# 编译paddlelite benchmark
# cmake -DTARGET_OS:STRING="android" -DTARGET_FRAMEWROK:STRING="paddlelite" -DCMAKE_TOOLCHAIN_FILE="/root/android_sdk/ndk/25.0.8775105/build/cmake/android.toolchain.cmake" -DANDROID_ABI="arm64-v8a" -DANDROID_PLATFORM=android-29 -G "Ninja" ..
# cmake --build . --target paddlelite_benchmark
# 编译onnxruntime benchmark

cmake -DBUILD_ONNXRUNTIME=ON -DBUILD_ANDROID=ON -DCMAKE_TOOLCHAIN_FILE="/root/android_sdk/ndk/25.0.8775105/build/cmake/android.toolchain.cmake" -DANDROID_ABI="arm64-v8a"  -DANDROID_PLATFORM=android-29 -G "Ninja" ..
cmake --build . --target ort_benchmark
# 编译ncnn benchmark

cmake -DBUILD_NCNN=ON -DBUILD_ANDROID=ON -DCMAKE_TOOLCHAIN_FILE="/root/android_sdk/ndk/25.0.8775105/build/cmake/android.toolchain.cmake" -DANDROID_ABI="arm64-v8a"  -DANDROID_PLATFORM=android-29 -G "Ninja" ..
cmake --build . --target ncnn_benchmark



cmake -DBUILD_MNN=ON -DBUILD_ANDROID=ON \
-DCMAKE_TOOLCHAIN_FILE="/root/android_sdk/ndk/25.0.8775105/build/cmake/android.toolchain.cmake" \
-DANDROID_ABI="arm64-v8a"  \
-DANDROID_PLATFORM=android-29 \
-G "Ninja" ..

# cmake -DBUILD_MNN=ON -DBUILD_LINUX_CPU=ON -G "Ninja" ..
cmake --build . --target mnn_benchmark
./mnn_benchmark --model="/root/workspace/UnifiedModelBenchmark/tmp/models/mnn/resnet50.mnn" \
--backend="X64

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/data/local/tmp/hcp/libs
```
cd 

## 2 使用

### onnxruntime benchmark


