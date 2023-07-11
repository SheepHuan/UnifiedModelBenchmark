#!/bin/bash
export adb_device="3a9c4f5"
export model_name="combined_resnet18-16"
export workspace="/data/local/tmp/hcp"

adb -s $adb_device shell "mkdir -p $workspace/models $workspace/libs"

adb -s $adb_device push 3rd-party/Paddle-Lite/build.lite.android.armv8.clang/inference_lite_lib.android.armv8.opencl/cxx/lib/libpaddle_full_api_shared.so $workspace/libs
# adb -s $adb_device push tmp/inference_lite_lib.android.armv8.clang.with_extra.with_log.full_publish/cxx/lib/libpaddle_full_api_shared.so $workspace/libs
# adb -s $adb_device push release/paddlelite_v2.13-arm_opencl/paddlelite_benchmark $workspace
adb -s $adb_device push build/paddlelite_benchmark $workspace

adb -s $adb_device push --sync models/$model_name $workspace/models
adb -s $adb_device shell "chmod +x $workspace/paddlelite_benchmark"
adb -s $adb_device shell "cd $workspace;
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/data/local/tmp/hcp/libs;
  ./paddlelite_benchmark --model=$workspace/models/$model_name/inference_model/model.pdmodel \
  --param=$workspace/models/$model_name/inference_model/model.pdiparams \
  --input_info=[x2paddle_test_input2,1x3x256x256,float32] --backend=arm --optimized_model_path=/data/local/tmp/mobile"