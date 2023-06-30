#!/bin/bash
export adb_device="3a9c4f5"
export benchmark_path="3rd-party/Paddle-Lite/build.lite.android.armv8.clang/inference_lite_lib.android.armv8/bin/benchmark_bin"
export model_path="models/MobileNetV1"
export workspace="/data/local/tmp/hcp"

adb -s $adb_device shell "mkdir -p $workspace/models"
adb -s $adb_device push $model_path $workspace/models
adb -s $adb_device push $benchmark_path $workspace
adb -s $adb_device shell "chmod +x $workspace/benchmark_bin"
adb -s $adb_device shell "cd $workspace;
  ./benchmark_bin \
    --model_file=models/MobileNetV1/inference.pdmodel \
    --param_file=models/MobileNetV1/inference.pdiparams \
    --input_shape=1,3,224,224 \
    --warmup=10 \
    --repeats=20 \
    --backend=arm"