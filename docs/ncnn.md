
```bash
git submodule update --init
cd /root/workspace/UnifiedModelBenchmark/3rd-party/ncnn
mkdir -p build-android-aarch64
cd build-android-aarch64

export ANDROID_NDK=/root/android_sdk/ndk/25.0.8775105
cmake -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK/build/cmake/android.toolchain.cmake"\
    -DANDROID_ABI="arm64-v8a" \
    -DANDROID_PLATFORM=android-21 ..

# If you want to enable Vulkan, platform api version >= android-24 is needed
cmake -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK/build/cmake/android.toolchain.cmake" \
    -DANDROID_ABI="arm64-v8a" \
    -DANDROID_PLATFORM=android-24 -DNCNN_VULKAN=ON ..

# If you use cmake >= 3.21 and ndk-r23
# you need to add -DANDROID_USE_LEGACY_TOOLCHAIN_FILE=False option for working optimization flags

make -j16
make install

```