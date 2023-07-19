#!/bin/bash
export adb_device="3a9c4f5"
export workspace="/data/local/tmp/hcp"

adb -s $adb_device shell "mkdir -p $workspace/models $workspace/libs"

adb -s $adb_device push --sync release/ncnn-20230517-android-vulkan-shared/arm64-v8a/lib/libncnn.so $workspace/libs
adb -s $adb_device push build/ncnn_benchmark $workspace

adb -s $adb_device push --sync tmp/ncnn/ncnn_resenet50.bin $workspace/models
adb -s $adb_device push --sync tmp/ncnn/ncnn_resenet50.params $workspace/models
# adb -s $adb_device shell "chmod +x $workspace/ncnn_benchmark"
adb -s $adb_device shell "cd $workspace;
    export LD_LIBRARY_PATH=/data/local/tmp/hcp/libs;
  ./ncnn_benchmark --model=$workspace/models/ncnn_resenet50.bin \
  --param=$workspace/models/ncnn_resenet50.params \
  --input_info=[input,1x3x256x256,float32] --backend=arm"