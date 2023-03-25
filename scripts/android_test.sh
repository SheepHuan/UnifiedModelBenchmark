#！/bin/bash
ANDROID_ADDR="172.16.101.184:5555"
ROOT_DIR="/workspace/UnifiedModelBenchmark"
DST_DIR="/data/local/tmp/hcp"
RUN_ARGS="--model_path="$DST_DIR/samples/yolov4.onnx" --image_path="$DST_DIR/samples/dog.jpg" --use_nnapi=true"
adb shell "mkdir -p $DST_DIR && mkdir -p $DST_DIR/libs"
adb push $ROOT_DIR/build/main $DST_DIR
adb push --sync $ROOT_DIR/samples/ $DST_DIR
adb push --sync $ROOT_DIR/libs/onnxruntime/android/arm64-v8a/* "$DST_DIR/libs"
adb push --sync $ROOT_DIR/libs/gflags/android/arm64-v8a/* "$DST_DIR/libs"
adb push --sync $ROOT_DIR/libs/opencv/android/arm64-v8a/4.7.0/* "$DST_DIR/libs"
adb shell "cd $DST_DIR && chmod +x main && export LD_LIBRARY_PATH=$DST_DIR/libs && ./main $RUN_ARGS"
