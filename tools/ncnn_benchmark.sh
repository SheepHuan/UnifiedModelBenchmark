#!/bin/bash
export adb_device="3a9c4f5"
export model_name="ncnn_resnet50-opset16"
export workspace="/data/local/tmp/hcp"

adb -s $adb_device shell "mkdir -p $workspace/models $workspace/libs"

adb -s $adb_device push --sync release/20230705_android_arm64-v8a/ncnn_20230517-arm_vulkan/lib/libncnn.so $workspace/libs
# adb -s $adb_device push release/paddlelite_v2.13-arm_opencl/paddlelite_benchmark $workspace
adb -s $adb_device push build/ncnn_benchmark $workspace

adb -s $adb_device push --sync models/$model_name $workspace/models
adb -s $adb_device shell "chmod +x $workspace/ncnn_benchmark"
adb -s $adb_device shell "cd $workspace;
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/data/local/tmp/hcp/libs;
  ./ncnn_benchmark --model=$workspace/models/$model_name/model.bin \
  --param=$workspace/models/$model_name/model.param \
  --input_info=[input,1x3x256x256,float32] --backend=arm"