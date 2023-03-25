#！/bin/bash
ANDROID_ADDR="172.16.101.185:38979"
ROOT_DIR="/workspace/UnifiedModelBenchmark"
DST_DIR="/data/local/tmp/hcp"
RUN_ARGS=""
adb shell "mkdir -p $DST_DIR"
adb push $ROOT_DIR/build/main $DST_DIR
adb push --sync $ROOT_DIR/samples/ $DST_DIR
adb shell "cd $DST_DIR && chmod +x main && ./main $RUN_ARGS"
