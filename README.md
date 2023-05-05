# Benchmark

## 编译指令
### Linux
```bash
# linux
cd build
cmake -DTARGET_OS:STRING="linux" ..
cmake --build . --target main

# linux 交叉编译 android 可执行文件
cmake -DTARGET_OS:STRING="android" -DCMAKE_TOOLCHAIN_FILE="/root/android_sdk/ndk/25.0.8775105/build/cmake/android.toolchain.cmake" -DANDROID_ABI="arm64-v8a" -DANDROID_PLATFORM=android-29 -G "Ninja" ..
cmake --build . --target main
```
### Android


### Windows


## 运行demo

```bash
./main --model_path="/workspace/UnifiedModelBenchmark/samples/yolov4.onnx" --image_path="/workspace/UnifiedModelBenchmark/samples/dog.jpg"


./main --model_path="/root/workspace/UnifiedModelBenchmark/models/yolov4.onnx" --image_shape="1x3x416x416" --image_path="/root/workspace/UnifiedModelBenchmark/samples/dog.jpg"
```



## 代码依赖库

1. onnxruntime@v1.14.1
2. opencv@v4.7.0
3. [gflags@2.2.2](https://github.com/gflags/gflags)

## TODO

### onnxruntime

0. ~~添加 arm android cpu,nnapi 支持~~
1. 添加 intel openvino cpu,gpu,vpu 支持
2. 添加 瑞芯微 npu 支持

### paddlitelite

### tensorflowlite

## 参考文章

[1] [C++设计实现日志系统](https://zhuanlan.zhihu.com/p/100082717)
