# onnx用ncnn推理

## linux
先已编译3.20.x的protobuf https://github.com/protocolbuffers/protobuf/blob/3.20.x/src/README.md
```bash
sudo apt install build-essential git cmake libprotobuf-dev protobuf-compiler libvulkan-dev vulkan-utils libopencv-dev
/root/workspace/UnifiedModelBenchmark/3rd-party/ncnn
mkdir build
cd build
cmake ..
make -j16
```


## android编译ncnn
https://github.com/Tencent/ncnn/wiki/vulkan-notes


```bash
git submodule update --init
cd /root/workspace/UnifiedModelBenchmark/3rd-party/ncnn
mkdir -p build-android-aarch64
cd build-android-aarch64

export ANDROID_NDK=/root/android_sdk/ndk/25.0.8775105
cmake -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK/build/cmake/android.toolchain.cmake"\
    -DANDROID_ABI="arm64-v8a" \
    -DANDROID_PLATFORM=android-29 \
    -DNCNN_SHARED_LIB=ON ..

# If you want to enable Vulkan, platform api version >= android-24 is needed
cmake -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK/build/cmake/android.toolchain.cmake" \
    -DANDROID_ABI="arm64-v8a" \
    -DANDROID_PLATFORM=android-24 -DNCNN_VULKAN=ON -DNCNN_SHARED_LIB=ON  ..

# If you use cmake >= 3.21 and ndk-r23
# you need to add -DANDROID_USE_LEGACY_TOOLCHAIN_FILE=False option for working optimization flags

make -j16
make install


mkdir -p build-android-aarch64-vulkan
cd build-android-aarch64-vulkan

export ANDROID_NDK=/root/android_sdk/ndk/25.0.8775105

# If you want to enable Vulkan, platform api version >= android-24 is needed
cmake -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK/build/cmake/android.toolchain.cmake" \
    -DANDROID_ABI="arm64-v8a" \
    -DANDROID_PLATFORM=android-29 \
    -DNCNN_VULKAN=ON \
    -DNCNN_SHARED_LIB=ON ..
make -j16
make install
```


## 2 .onnx转ncnn格式

### 2.1 先用onnx-simplifier简化onnx模型
```bash
pip3 install -U pip && pip3 install onnxsim

onnxsim input_onnx_model output_onnx_model
# onnxsim /root/workspace/UnifiedModelBenchmark/models/fusenet_large-opset16.onnx /root/workspace/UnifiedModelBenchmark/models/fusenet_large-opset16_simple.onnx
```
```text
Simplifying...
Finish! Here is the difference:
┏━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━┓
┃                   ┃ Original Model ┃ Simplified Model ┃
┡━━━━━━━━━━━━━━━━━━━╇━━━━━━━━━━━━━━━━╇━━━━━━━━━━━━━━━━━━┩
│ Add               │ 113            │ 113              │
│ Concat            │ 3              │ 2                │
│ Constant          │ 674            │ 656              │
│ Conv              │ 315            │ 315              │
│ Div               │ 6              │ 6                │
│ Gemm              │ 5              │ 5                │
│ GlobalAveragePool │ 1              │ 1                │
│ MaxPool           │ 3              │ 3                │
│ Mul               │ 8              │ 8                │
│ Pow               │ 6              │ 6                │
│ ReduceMax         │ 1              │ 1                │
│ ReduceMean        │ 13             │ 13               │
│ Relu              │ 304            │ 304              │
│ Reshape           │ 9              │ 7                │
│ Sigmoid           │ 2              │ 2                │
│ Slice             │ 2              │ 2                │
│ Sqrt              │ 6              │ 6                │
│ Sub               │ 6              │ 6                │
│ Model Size        │ 534.3MiB       │ 534.3MiB         │
└───────────────────┴────────────────┴──────────────────┘
```
### 2.2 ncnn转换
```bash
cd /root/workspace/UnifiedModelBenchmark/3rd-party/ncnn/build/tools/onnx
./onnx2ncnn /root/workspace/UnifiedModelBenchmark/models/fusenet_large-opset16_simple.onnx /root/workspace/UnifiedModelBenchmark/models/fusenet_large_simple.param /root/workspace/UnifiedModelBenchmark/models/fusenet_large_simple.bin


```