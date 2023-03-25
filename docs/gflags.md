# gflags

## 编译

```bash
# linux
mkdir build && cd build
mkdir install
cmake -DCMAKE_INSTALL_PREFIX=/workspace/gflags/build/install -DBUILD_SHARED_LIBS=ON ..

# android
mkdir build && cd build
mkdir install
cmake  -DCMAKE_TOOLCHAIN_FILE="/root/android_sdk/ndk/25.0.8775105/build/cmake/android.toolchain.cmake" -DBUILD_SHARED_LIBS=ON -DANDROID_ABI="arm64-v8a" -DANDROID_PLATFORM=android-29 -DGFLAGS_NAMESPACE=gflags -DCMAKE_CXX_FLAGS=-fPIC ..

make
```
