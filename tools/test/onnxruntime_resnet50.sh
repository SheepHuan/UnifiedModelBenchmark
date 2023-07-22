#!/bin/bash
export device="192.168.1.105:5555"
export device_dir="/data/local/tmp/hcp"

adb -s $device shell "mkdir -p $device_dir/models $device_dir/libs"
adb -s $device push --sync /root/workspace/UnifiedModelBenchmark/release/onnxruntime-v1.15.1-android-shared/lib/libonnxruntime.so $device_dir/libs
adb -s $device push --sync /root/workspace/UnifiedModelBenchmark/tmp/models/resnet50-opset16.onnx $device_dir/models
adb -s $device push --sync /root/workspace/UnifiedModelBenchmark/build/ort_benchmark $device_dir

adb -s $device shell "export LD_LIBRARY_PATH=/data/local/tmp/hcp/libs && .$device_dir/ort_benchmark \
--model=$device_dir/models/resnet50-opset16.onnx \
--backend=arm --num_warmup=10 --num_runs=50 --num_threads=4 --enable_op_profiling=true --prefix=/data/local/tmp/hcp/1"
