#!/bin/bash
# export device="3a9c4f5"
export device="3a9c4f5"
export device_dir="/data/local/tmp/hcp"
export benchmark_path="/root/workspace/UnifiedModelBenchmark/build/ort_benchmark"


adb -s $device shell "mkdir -p $device_dir/models $device_dir/libs"
adb -s $device push --sync /root/workspace/UnifiedModelBenchmark/libs/test/libonnxruntime.so $device_dir/libs
adb -s $device push --sync /root/workspace/UnifiedModelBenchmark/tmp/models/resnet50-opset16.onnx $device_dir/models
adb -s $device push --sync $benchmark_path $device_dir

adb -s $device shell "export LD_LIBRARY_PATH=/data/local/tmp/hcp/libs && .$device_dir/ort_benchmark \
--model=$device_dir/models/resnet50-opset16.onnx \
--backend=arm --num_warmup=10 --num_runs=50 --num_threads=4 --enable_op_profiling=true --prefix=/data/local/tmp/hcp/1"

# adb -s $device shell "export LD_LIBRARY_PATH=$device_dir/libs ;cd $device_dir; ./ort_benchmark"

# export LD_LIBRARY_PATH='/data/local/tmp/hcp/libs'
# ldd /data/local/tmp/hcp/ort_benchmark
# LD_LIBRARY_PATH=libs/ ./ort_benchmark --model=/data/local/tmp/hcp/models/sim-mobilestereonet2d-opset16.onnx --backend=arm