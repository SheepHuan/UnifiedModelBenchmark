

```bash
# 1.
export https_proxy="http://172.16.101.124:7890"
git clone -b 20230517 https://github.com/Tencent/ncnn.git
cd ncnn
git submodule update --init
```


## android vulkan编译

```bash
export ANDROID_NDK=/root/android_sdk/ndk/25.0.8775105

mkdir -p build-android-aarch64-vulkan
cd build-android-aarch64-vulkan

cmake -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK/build/cmake/android.toolchain.cmake" \
    -DANDROID_ABI="arm64-v8a" \
    -DANDROID_PLATFORM=android-29 \
    -DNCNN_VULKAN=ON \
    -DNCNN_SHARED_LIB=ON ..

make -j16
make install
```

此时在`ncnn/build-android-aarch64-vulkan/install`下面有如下文件
```text

|-- include
|   `-- ncnn
|       |-- allocator.h
|       |-- benchmark.h
|       ...
|       `-- vulkan_header_fix.h
`-- lib
    |-- libncnn.so
```


## ubuntu 编译

```bash

mkdir -p build-ubuntu-amd64
cd build-ubuntu-amd64

cmake ..

make -j16
make install
```


### onnx 2 ncnn

```
cd models/
onnxsim resnet50-opset16.onnx sim_resnet50-opset16.onnx

../release/20230705_android_arm64-v8a/ncnn_20230517-arm_vulkan/tools/onnx2ncnn sim_resnet50-opset16.onnx ncnn_resnet50-opset16.param ncnn_resnet50-opset16.bin
```