# Benchmark

```bash
# linux
cd build
cmake -DTARGET_OS:STRING="linux" ..
cmake --build . --target main

# linux 交叉编译 android 可执行文件
cmake -DTARGET_OS:STRING="android" -DCMAKE_TOOLCHAIN_FILE="/root/android_sdk/ndk/25.0.8775105/build/cmake/android.toolchain.cmake" -DANDROID_ABI="arm64-v8a" -DANDROID_PLATFORM=android-29 -G "Ninja" ..
cmake --build . --target main
```


## 编译指令
### Linux

### Android


### Windows


## 运行demo

```bash
./main --model_path="/workspace/UnifiedModelBenchmark/samples/yolov4.onnx" --image_path="/workspace/UnifiedModelBenchmark/samples/dog.jpg"


./main --model_path="/root/workspace/UnifiedModelBenchmark/models/yolov4.onnx" --image_shape="1x3x416x416" --image_path="/root/workspace/UnifiedModelBenchmark/samples/dog.jpg"
```



## 代码依赖库

1. [onnxruntime@v1.14.1](https://github.com/microsoft/onnxruntime.git)
2. [opencv@v4.7.0](https://github.com/opencv/opencv.git )
3. [gflags@2.2.2](https://github.com/gflags/gflags)
```bash
git submodule add https://github.com/gflags/gflags.git 3rd-party/gflags
git submodule add https://github.com/opencv/opencv.git 3rd-party/opencv
git submodule add https://github.com/microsoft/onnxruntime.git 3rd-party/onnxruntime
# 确认子模块
git submodule update --init --recursive
```

## TODO
1. 学会如何将opencv作为子模块加入本项目,简化编译流程(blog记录)
2. 支持onnxruntime android端推理
3. 支持tensorflow lite android端推理
4. 支持paddle lite android端推理
 


## 参考文章

[1] [C++设计实现日志系统](https://zhuanlan.zhihu.com/p/100082717)
