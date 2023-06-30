#!/bin/bash
export adb_device="3a9c4f5"
export benchmark_path="build/paddlelite_benchmark"
export model_path="models/MobileNetV1"
export workspace="/data/local/tmp/hcp"

adb -s $adb_device shell "mkdir -p $workspace/models $workspace/libs"

adb -s $adb_device push --sync release/paddlelite_v2.13-arm_opencl/*.so $workspace/libs

adb -s $adb_device push --sync $model_path $workspace/models
adb -s $adb_device push  $benchmark_path $workspace
adb -s $adb_device shell "chmod +x $workspace/paddlelite_benchmark"
adb -s $adb_device shell "cd $workspace;
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/data/local/tmp/hcp/libs;
  ./paddlelite_benchmark --model=$workspace/models/MobileNetV1/inference.pdmodel\
  --param=$workspace/models/MobileNetV1/inference.pdiparams\
  --input_info=inputs:1x3x224x224\
  --backend=arm\
  --optimized_model_dir=/data/local/tmp/mobile"