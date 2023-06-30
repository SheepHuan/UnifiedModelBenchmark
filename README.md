# Model Benchmark

## 1 安装

### 编译benchmark
准备好依赖后直接
```bash
# linux 交叉编译 android 可执行文件，要求先编译完依赖库onnxruntime.so paddlelite.so ncnn.so
# 编译paddlelite benchmark
cmake -DTARGET_OS:STRING="android" -DTARGET_FRAMEWROK:STRING="paddlelite" -DCMAKE_TOOLCHAIN_FILE="/root/android_sdk/ndk/25.0.8775105/build/cmake/android.toolchain.cmake" -DANDROID_ABI="arm64-v8a" -DANDROID_PLATFORM=android-29 -G "Ninja" ..
cmake --build . --target paddlelite_benchmark
# 编译onnxruntime benchmark
cmake -DTARGET_OS:STRING="android" -DTARGET_FRAMEWROK:STRING="onnxruntime" -DCMAKE_TOOLCHAIN_FILE="/root/android_sdk/ndk/25.0.8775105/build/cmake/android.toolchain.cmake" -DANDROID_ABI="arm64-v8a" -DANDROID_PLATFORM=android-29 -G "Ninja" ..
cmake --build . --target ort_benchmark
# 编译ncnn benchmark
cmake -DTARGET_OS:STRING="android" -DTARGET_FRAMEWROK:STRING="ncnn" -DCMAKE_TOOLCHAIN_FILE="/root/android_sdk/ndk/25.0.8775105/build/cmake/android.toolchain.cmake" -DANDROID_ABI="arm64-v8a"  -DANDROID_PLATFORM=android-29 -G "Ninja" ..
cmake --build . --target ncnn_benchmark
```


## 2 使用

### onnxruntime benchmark


### paddlelite benchmark


### ncnn benchmark
#### 参数
- `model`, 模型图`*.bin`的路径.
- `param`, 模型参数`*.param`的路径.
- `backend`, ort的推理后端,默认`arm`,可选`arm`,`vulkan`.
- `num_threads`, ort的后端为`arm`的时候，可以选择CPU推理数量`1,..,4,..,8`
- `nums_warmup`, 热身次数，不参与计时.
- `num_runs`, 推理次数.
- `input_info`, 输入张量的信息格式为"input1:1x3x128x128,input2:1x4x56x56".支持多输入
- `output_info`, 输出张量的信息格式为:"output1:1x7x128x128,output2:1x4x128x128".支持多输出.

