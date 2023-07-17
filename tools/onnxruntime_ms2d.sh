#!/bin/bash
# export device="3a9c4f5"
export device="3a9c4f5"
export device_dir="/data/local/tmp/hcp"
export benchmark_path="/root/workspace/UnifiedModelBenchmark/build/ort_benchmark"


adb -s $device shell "mkdir -p $device_dir/models $device_dir/libs"
adb -s $device push --sync /root/workspace/UnifiedModelBenchmark/libs/onnxruntime-v1.14.1-android_shared/libs/libonnxruntime.so $device_dir/libs
adb -s $device push --sync tmp/models/sim-mobilestereonet2d-opset16.onnx $device_dir/models
adb -s $device push --sync $benchmark_path $device_dir

adb -s $device shell "export LD_LIBRARY_PATH=$device_dir/libs && .$device_dir/ort_benchmark \
--model=$device_dir/models/sim-mobilestereonet2d-opset16.onnx \
--backend=arm --num_warmup=10 --num_runs=50"

# adb -s $device shell "export LD_LIBRARY_PATH=$device_dir/libs ;cd $device_dir; ./ort_benchmark"

# export LD_LIBRARY_PATH=/data/local/tmp/hcp/libs
# ldd /data/local/tmp/hcp/ort_benchmark
# LD_LIBRARY_PATH=libs/ ./ort_benchmark --model=/data/local/tmp/hcp/models/sim-mobilestereonet2d-opset16.onnx --backend=arm