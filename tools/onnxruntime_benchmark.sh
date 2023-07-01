#!/bin/bash
export adb_device="3a9c4f5"
export model_name="mobilenetv2-12.onnx"
export workspace="/data/local/tmp/hcp"

adb -s $adb_device shell "mkdir -p $workspace/models $workspace/libs"

adb -s $adb_device push --sync release/ort_v1.14.1-arm_nnapi/libonnxruntime.so $workspace/libs
adb -s $adb_device push release/ort_v1.14.1-arm_nnapi/ort_benchmark $workspace

adb -s $adb_device push --sync models/$model_name $workspace/models
adb -s $adb_device shell "chmod +x $workspace/ort_benchmark"
adb -s $adb_device shell "cd $workspace;
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/data/local/tmp/hcp/libs;
  ./ort_benchmark --graph=$workspace/models/$model_name \
  --backend=arm \
  --num_threads=4"