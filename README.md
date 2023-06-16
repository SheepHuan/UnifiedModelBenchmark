# Benchmark

## 编译指令
### Linux
```bash
# linux
cd build
cmake -DTARGET_OS:STRING="linux" ..
cmake --build . --target main 

./main --model_path="/root/workspace/UnifiedModelBenchmark/models/squeezenet1.0-12-int8.onnx" --prefix="1"
./main --model_path="/root/workspace/UnifiedModelBenchmark/models/FasterRCNN-12.onnx" --prefix="1"
./main --model_path="/root/workspace/UnifiedModelBenchmark/models/ssd-12.onnx" --prefix="1"
```

### Android
```bash
# linux 交叉编译 android 可执行文件，要求先编译完依赖库onnxruntime.so paddlelite.so ncnn.so
# 编译paddlelite benchmark
cmake -DTARGET_OS:STRING="android" -DTARGET_FRAMEWROK:STRING="paddlelite" -DCMAKE_TOOLCHAIN_FILE="/root/android_sdk/ndk/25.0.8775105/build/cmake/android.toolchain.cmake" -DANDROID_ABI="arm64-v8a" -DANDROID_PLATFORM=android-29 -G "Ninja" ..
cmake --build . --target paddlelite_benchmark
# 编译onnxruntime benchmark
cmake -DTARGET_OS:STRING="android" -DTARGET_FRAMEWROK:STRING="onnxruntime" -DCMAKE_TOOLCHAIN_FILE="/root/android_sdk/ndk/25.0.8775105/build/cmake/android.toolchain.cmake" -DANDROID_ABI="arm64-v8a" -DANDROID_PLATFORM=android-29 -G "Ninja" ..
cmake --build . --target ort_benchmark
# 编译ncnn benchmark
cmake -DTARGET_OS:STRING="android" -DTARGET_FRAMEWROK:STRING="ncnn" -DCMAKE_TOOLCHAIN_FILE="/root/android_sdk/ndk/25.0.8775105/build/cmake/android.toolchain.cmake" -DANDROID_ABI="arm64-v8a" -DANDROID_PLATFORM=android-29 -G "Ninja" ..
cmake --build . --target ncnn_benchmark
```

### Windows
```bash
adb -s 3a9c4f5 shell "mkdir -p /data/local/tmp/mobifuse /data/local/tmp/mobifuse/libs /data/local/tmp/mobifuse/models"
adb -s 3a9c4f5 push --sync /root/workspace/UnifiedModelBenchmark/3rd-party/onnxruntime/build/Android/Release/*.so /data/local/tmp/mobifuse/libs
adb -s 3a9c4f5 push --sync /root/workspace/UnifiedModelBenchmark/build/ort_benchmark /data/local/tmp/mobifuse

adb -s 3a9c4f5 push --sync /root/workspace/UnifiedModelBenchmark/models/fusenet_large-opset16.onnx /data/local/tmp/mobifuse/models

adb -s 3a9c4f5 shell 'export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/data/local/tmp/mobifuse/libs" && /data/local/tmp/mobifuse/ort_benchmark --graph="/data/local/tmp/mobifuse/models/fusenet_large-opset16.onnx" --nums_warmup=10 --num_runs=30 --num_threads=4'
```

## 运行demo
https://www.paddlepaddle.org.cn/lite/v2.12/user_guides/opt/opt_python.html
```bash
adb -s 3a9c4f5 push 3rd-party/Paddle-Lite/build.lite.android.armv8.clang/inference_lite_lib.android.armv8.opencl/cxx/lib/*.so /data/local/tmp/hcp/libs
adb -s 3a9c4f5 push models/mobilenet_v1_opt.nb /data/local/tmp/hcp/

adb -s 3a9c4f5 push build/aiot_benchmark /data/local/tmp/hcp/
adb -s 3a9c4f5 shell "chmod +x /data/local/tmp/hcp/aiot_benchmark"

adb -s 3a9c4f5 shell 'export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/data/local/tmp/hcp/libs" && /data/local/tmp/hcp/aiot_benchmark --graph="/data/local/tmp/hcp/MobileNetV1" --graph_is_dir=true --nums_warmnup=50 --nums_run=50 --num_threads=1 --cpu_power_mode=0 --backend=arm --input_shape=1,3,224,224'

adb -s 3a9c4f5 shell 'export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/data/local/tmp/hcp/libs" && /data/local/tmp/hcp/aiot_benchmark --graph="/data/local/tmp/hcp/MobileNetV1" --graph_is_dir=true --nums_warmnup=15 --nums_run=50 --num_threads=1 --cpu_power_mode=0 --backend=opencl --input_shape=1,3,224,224'

adb -s 3a9c4f5 shell 'export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/data/local/tmp/hcp/libs" && /data/local/tmp/hcp/aiot_benchmark'

paddle_lite_opt \
    --model_dir=models/mobilenetv1 \
    --valid_targets=arm \
    --optimize_out=mobilenet_v1_opt
```

```bash




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
 


## 参考代码

[1] https://github.com/rxi/log.c/
