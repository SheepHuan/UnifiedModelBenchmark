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

## 链接到本仓库的子项目

```bash
git submodule add <子项目的仓库地址> <本地目录>
git submodule add https://github.com/gflags/gflags.git 3rd-party/gflags
git submodule add https://github.com/opencv/opencv.git 3rd-party/opencv

```
