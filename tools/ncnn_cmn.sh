#!/bin/bash
export device="3a9c4f5"

export device_dir="/data/local/tmp/hcp"
export benchmark_path="/root/workspace/UnifiedModelBenchmark/build/ncnn_benchmark"
export lib_path="/root/workspace/UnifiedModelBenchmark/libs/ncnn-20230517-android-vulkan-shared/arm64-v8a/lib/libncnn.so"

adb -s $device push --sync $lib_path $device_dir/libs
adb -s $device push --sync tmp/ncnn/cmn.bin $device_dir/models
adb -s $device push --sync tmp/ncnn/cmn.param $device_dir/models
adb -s $device push --sync $benchmark_path $device_dir

adb -s $device shell "export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:$device_dir/libs && .$device_dir/ncnn_benchmark \
--model=$device_dir/models/cmn.bin \
-param=$device_dir/models/cmn.param \
--backend=arm \
--input_info=[input,1x65x180x240,float32]"