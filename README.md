# Benchmark

## 1 编译指令
### 1.1 Linux
```bash
# linux
cd build
cmake -DTARGET_OS:STRING="linux" ..
cmake --build . --target main 

./main --model_path="/root/workspace/UnifiedModelBenchmark/models/squeezenet1.0-12-int8.onnx" --prefix="1"
./main --model_path="/root/workspace/UnifiedModelBenchmark/models/FasterRCNN-12.onnx" --prefix="1"
./main --model_path="/root/workspace/UnifiedModelBenchmark/models/ssd-12.onnx" --prefix="1"


# ncnn
mkdir 
```

### 1.2 Linux交叉编译Android
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

## 2 demo

### 2.1 onnxruntime 运行 .onnx
#### 2.1.1 ort_benchmark 参数解释

- `graph`, 模型图的路径.
- `backend`, ort的推理后端,默认`arm`,可选`arm`,`nnapi`.
- `num_threads`, ort的后端为`arm`的时候，可以选择CPU推理数量`1,..,4,..,8`
- `nums_warmup`, 热身次数，不参与计时.
- `num_runs`, 推理次数.
- `enable_op_profiling`, 是否打开op profiling,这将会保存文件到当前目录,默认是`false`
- `prefix`, 当`enable_op_profiling=true`时，可以设置`prefix`作为文件前缀，改变保存目录。

#### 2.1.2 demo
```bash
# 1. 创建libs,models文件夹
adb -s 3a9c4f5 shell "mkdir -p /data/local/tmp/mobifuse /data/local/tmp/mobifuse/libs /data/local/tmp/mobifuse/models"
# 2. 推送编译好的onnxruntime.so以及相关.a的库文件到anroid端
adb -s 3a9c4f5 push --sync /root/workspace/UnifiedModelBenchmark/3rd-party/onnxruntime/build/Android/Release/*.so /data/local/tmp/mobifuse/libs
# 3. 推送编译好的benchmark可执行程序到android端
adb -s 3a9c4f5 push --sync /root/workspace/UnifiedModelBenchmark/build/ort_benchmark /data/local/tmp/mobifuse

# 4. 推送模型文件到android端
adb -s 3a9c4f5 push --sync /root/workspace/UnifiedModelBenchmark/models/fusenet_large-opset16.onnx /data/local/tmp/mobifuse/models

# 5. 执行adb shell指令,backend cpu
adb -s 3a9c4f5 shell 'export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/data/local/tmp/mobifuse/libs" && /data/local/tmp/mobifuse/ort_benchmark --graph="/data/local/tmp/mobifuse/models/fusenet_large-opset16.onnx" --backend=cpu --nums_warmup=10 --num_runs=30 --num_threads=4'

# 5. 执行adb shell指令,backend nnapi
adb -s 3a9c4f5 shell 'export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/data/local/tmp/mobifuse/libs" && /data/local/tmp/mobifuse/ort_benchmark --graph="/data/local/tmp/mobifuse/models/fusenet_large-opset16.onnx" --backend=nnapi --nums_warmup=10 --num_runs=30 --num_threads=4 --enable_op_profiling=true --prefix="/data/local/tmp/mobifuse/nnapi"'

```



### 2.2 ncnn 运行 .onnx
```bash
# 1. 创建libs,models文件夹
adb -s 3a9c4f5 shell "mkdir -p /data/local/tmp/mobifuse /data/local/tmp/mobifuse/libs /data/local/tmp/mobifuse/models"
# 2. 推送编译好的onnxruntime.so以及相关.a的库文件到anroid端
adb -s 3a9c4f5 push --sync /root/workspace/UnifiedModelBenchmark/3rd-party/ncnn/build-android-aarch64/install/lib/*.so /data/local/tmp/mobifuse/libs

adb -s 3a9c4f5 push --sync /root/workspace/UnifiedModelBenchmark/3rd-party/ncnn/build-android-aarch64-vulkan/install/lib/*.so /data/local/tmp/mobifuse/libs
# 3. 推送编译好的benchmark可执行程序到android端
adb -s 3a9c4f5 push --sync /root/workspace/UnifiedModelBenchmark/build/ncnn_benchmark /data/local/tmp/mobifuse

# 4. 推送模型文件到android端
adb -s 3a9c4f5 push --sync /root/workspace/UnifiedModelBenchmark/models/fusenet_large_simple.bin /data/local/tmp/mobifuse/models
adb -s 3a9c4f5 push --sync /root/workspace/UnifiedModelBenchmark/models/fusenet_large_simple.param /data/local/tmp/mobifuse/models
# 5. 执行adb shell指令, arm cpu
adb -s 3a9c4f5 shell 'export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/data/local/tmp/mobifuse/libs" && /data/local/tmp/mobifuse/ncnn_benchmark --graph="/data/local/tmp/mobifuse/models/fusenet_large_simple.bin" --param="/data/local/tmp/mobifuse/models/fusenet_large_simple.param" --nums_warmup=10 --num_runs=30 --num_threads=4 --input_info="input:1x4x128x128" --output_info="output:1x7x128x128,feat_out:1x4x128x128"'

# 5. 执行adb shell指令, vulkan gpu
adb -s 3a9c4f5 shell 'export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/data/local/tmp/mobifuse/libs" && /data/local/tmp/mobifuse/ncnn_benchmark --graph="/data/local/tmp/mobifuse/models/fusenet_large_simple.bin" --param="/data/local/tmp/mobifuse/models/fusenet_large_simple.param" --nums_warmup=10 --num_runs=30 --num_threads=4 --backend=vulkan --input_info="input:1x4x128x128" --output_info="output:1x7x128x128,feat_out:1x4x128x128"'
```



### 2.3 paddlelite 运行 .pb
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



## TODO
1. 学会如何将opencv作为子模块加入本项目,简化编译流程(blog记录)
2. 支持onnxruntime android端推理,DONE
3. 支持tensorflow lite android端推理
4. 支持paddle lite android端推理,DONE
5. 支持ncnn lite android端推理,DONE
 


## 参考代码

[1] https://github.com/abumq/easyloggingpp/
[2] [onnxruntime@v1.14.1](https://github.com/microsoft/onnxruntime.git)
[3] [gflags@2.2.2](https://github.com/gflags/gflags)
[4] [ncnn@20230327](https://github.com/Tencent/ncnn)
[5] [onnx-simplifier](https://github.com/daquexian/onnx-simplifier/blob/master/README.md)