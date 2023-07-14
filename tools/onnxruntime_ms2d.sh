#!/bin/bash
export device="3a9c4f5"

export device_dir="/data/local/tmp/hcp"
export benchmark_path="/root/workspace/UnifiedModelBenchmark/build/ort_benchmark"
export lib_path="/root/workspace/UnifiedModelBenchmark/libs/onnxruntime-v1.15.1-android-shared/*"


adb -s $device shell "mkdir -p $device_dir/models $device_dir/libs"

adb -s $device push --sync $lib_path $device_dir/libs
adb -s $device push --sync tmp/models/sim-mobilestereonet2d-opset16.onnx $device_dir/models
adb -s $device push --sync $benchmark_path $device_dir

adb -s $device shell "export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:$device_dir/libs ; .$device_dir/ort_benchmark \
--model=$device_dir/models/sim-mobilestereonet2d-opset16.onnx \
--backend=arm"