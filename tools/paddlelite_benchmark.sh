#!/bin/bash
export adb_device="3a9c4f5"
export model_name="MobileNetV1"
export workspace="/data/local/tmp/hcp"

adb -s $adb_device shell "mkdir -p $workspace/models $workspace/libs"

adb -s $adb_device push --sync release/paddlelite_v2.13-arm_opencl/libpaddle_full_api_shared.so $workspace/libs
# adb -s $adb_device push release/paddlelite_v2.13-arm_opencl/paddlelite_benchmark $workspace
adb -s $adb_device push build/paddlelite_benchmark $workspace

adb -s $adb_device push --sync models/$model_name $workspace/models
adb -s $adb_device shell "chmod +x $workspace/paddlelite_benchmark"
adb -s $adb_device shell "cd $workspace;
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/data/local/tmp/hcp/libs;
  ./paddlelite_benchmark --model=$workspace/models/$model_name/inference.pdmodel \
  --param=$workspace/models/$model_name/inference.pdiparams \
  --input_info=[inputs,1x3x224x224,float32] --backend=opencl --optimized_model_path=/data/local/tmp/mobile"