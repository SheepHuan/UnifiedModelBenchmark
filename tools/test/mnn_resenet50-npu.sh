#!/bin/bash
export device="192.168.1.105:5555"
export device_dir="/data/local/tmp/hcp"

adb -s $device shell "mkdir -p $device_dir/models $device_dir/libs"
adb -s $device push --sync /root/workspace/UnifiedModelBenchmark/release/mnn_2.6.0-android_armv8_cpu_opencl_vulkan_npu/lib/libMNN_CL.so $device_dir/libs
adb -s $device push --sync /root/workspace/UnifiedModelBenchmark/release/mnn_2.6.0-android_armv8_cpu_opencl_vulkan_npu/lib/libMNN_Express.so $device_dir/libs
adb -s $device push --sync /root/workspace/UnifiedModelBenchmark/release/mnn_2.6.0-android_armv8_cpu_opencl_vulkan_npu/lib/libMNN_Vulkan.so $device_dir/libs
adb -s $device push --sync /root/workspace/UnifiedModelBenchmark/release/mnn_2.6.0-android_armv8_cpu_opencl_vulkan_npu/lib/libMNN.so $device_dir/libs
adb -s $device push --sync /root/workspace/UnifiedModelBenchmark/release/mnn_2.6.0-android_armv8_cpu_opencl_vulkan_npu/lib/libMNN_NPU.so $device_dir/libs
adb -s $device push --sync /root/workspace/UnifiedModelBenchmark/release/mnn_2.6.0-android_armv8_cpu_opencl_vulkan_npu/lib/libhiai.so $device_dir/libs
adb -s $device push --sync /root/workspace/UnifiedModelBenchmark/release/mnn_2.6.0-android_armv8_cpu_opencl_vulkan_npu/lib/libhiai_ir.so $device_dir/libs
adb -s $device push --sync /root/workspace/UnifiedModelBenchmark/release/mnn_2.6.0-android_armv8_cpu_opencl_vulkan_npu/lib/libhiai_ir_build.so $device_dir/libs


adb -s $device push --sync /root/workspace/UnifiedModelBenchmark/tmp/models/mnn/resnet50.mnn $device_dir/models
adb -s $device push --sync /root/workspace/UnifiedModelBenchmark/build/mnn_benchmark $device_dir
adb -s $device shell "chmod +x $device_dir/mnn_benchmark"
adb -s $device shell "export LD_LIBRARY_PATH=/data/local/tmp/hcp/libs ; cd $device_dir; ./mnn_benchmark \
--model=$device_dir/models/resnet50.mnn \
--backend=opencl --num_warmup=10 --num_runs=50 --num_threads=4 --cl_mem_type=cl_mem_image --mnn_precision=2"