#!/bin/bash
export device="192.168.1.105:5555"
export device_dir="/data/local/tmp/hcp"

adb -s $device shell "mkdir -p $device_dir/models $device_dir/libs"
adb -s $device push --sync /root/workspace/UnifiedModelBenchmark/release/mnn_2.6.0_android_armv7_armv8_cpu_opencl_vulkan/arm64-v8a/libMNN_CL.so $device_dir/libs
adb -s $device push --sync /root/workspace/UnifiedModelBenchmark/release/mnn_2.6.0_android_armv7_armv8_cpu_opencl_vulkan/arm64-v8a/libMNN_Express.so $device_dir/libs
adb -s $device push --sync /root/workspace/UnifiedModelBenchmark/release/mnn_2.6.0_android_armv7_armv8_cpu_opencl_vulkan/arm64-v8a/libMNN_Vulkan.so $device_dir/libs
adb -s $device push --sync /root/workspace/UnifiedModelBenchmark/release/mnn_2.6.0_android_armv7_armv8_cpu_opencl_vulkan/arm64-v8a/libMNN.so $device_dir/libs
adb -s $device push --sync /root/workspace/UnifiedModelBenchmark/release/mnn_2.6.0_android_armv7_armv8_cpu_opencl_vulkan/arm64-v8a/libmnncore.so $device_dir/libs

adb -s $device push --sync /root/workspace/UnifiedModelBenchmark/tmp/models/mnn/resnet50.mnn $device_dir/models
adb -s $device push --sync /root/workspace/UnifiedModelBenchmark/build/mnn_benchmark $device_dir
adb -s $device shell "chmod +x $device_dir/mnn_benchmark"
adb -s $device shell "export LD_LIBRARY_PATH=/data/local/tmp/hcp/libs ; cd $device_dir; ./mnn_benchmark \
--model=$device_dir/models/resnet50.mnn \
--backend=nnapi --num_warmup=10 --num_runs=50 --num_threads=4"