#！/bin/bash
ANDROID_ADDR="172.16.101.184:5555"
ROOT_DIR="/workspace/UnifiedModelBenchmark"
DST_DIR="/data/local/tmp/hcp"
MODEL_NAME="yolov4.onnx"

RUN_ARGS="--model_path="$DST_DIR/models/$MODEL_NAME" --image_path="$DST_DIR/samples/dog.jpg" --use_nnapi=true --image_shape=1x3x416x416"
adb -s $ANDROID_ADDR shell "mkdir -p $DST_DIR && mkdir -p $DST_DIR/libs"
adb -s $ANDROID_ADDR push $ROOT_DIR/build/main $DST_DIR
adb -s $ANDROID_ADDR push --sync $ROOT_DIR/models/ $DST_DIR
adb -s $ANDROID_ADDR push --sync $ROOT_DIR/libs/onnxruntime/android/arm64-v8a/* "$DST_DIR/libs"
adb -s $ANDROID_ADDR push --sync $ROOT_DIR/libs/gflags/android/arm64-v8a/* "$DST_DIR/libs"
adb -s $ANDROID_ADDR push --sync $ROOT_DIR/libs/opencv/android/arm64-v8a/4.7.0/* "$DST_DIR/libs"
adb -s $ANDROID_ADDR shell "cd $DST_DIR && chmod +x main && export LD_LIBRARY_PATH=$DST_DIR/libs && ./main $RUN_ARGS"
