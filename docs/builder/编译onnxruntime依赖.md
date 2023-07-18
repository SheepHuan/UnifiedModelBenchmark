## 环境准备

1. apt安装
```bash

# gcc等编译工具
apt install libssl-dev build-essential
# 可能需要java环境
apt install openjdk-11-jdk
# 编译需要python3
apt install python3.8
ln -s /usr/bin/python3.8 /usr/bin/python3
# cmake环境
wget https://github.com/Kitware/CMake/releases/download/v3.26.0/cmake-3.26.0-linux-x86_64.sh

# 配置环境变量即可...
```

2. 进入[Android Studio 网址](https://developer.android.com/studio),下载"Command line tools only"
```bash
wget https://dl.google.com/android/repository/commandlinetools-linux-9477386_latest.zip
unzip commandlinetools-linux-9477386_latest.zip
cp cmdline-tools/ ~/android_sdk

# 安装 android 环境
sdkmanager --install "platforms;android-29"
sdkmanager --install "ndk;25.0.8775105"
```


## 编译推理库

```bash
# 推荐设置代理
export https_proxy="http://172.16.6.15:7891"
export http_proxy="http://172.16.6.15:7891"
export ANDROID_NDK="/root/android_sdk/ndk/25.0.8775105"
export PATH=$PATH:/root/android_sdk/cmake/3.22.1/bin

git clone  --recursive -b v1.14.1 https://github.com/microsoft/onnxruntime.git
cd onnxruntime 
git submodule sync --recursive
# 编译
./build.bat --android --android_sdk_path .../Android --android_ndk_path .../Android/ndk/21.1.6352462 --android_abi arm64-v8a --android_api 27

./build.sh --config Release --parallel --build_shared_lib --android --android_sdk_path  /root/android_sdk --android_ndk_path /root/android_sdk/ndk/23.2.8568313 --android_abi arm64-v8a --android_api 27

./build.sh --config Release --parallel --build_shared_lib --android --android_sdk_path /root/android_sdk --android_ndk_path /root/android_sdk/ndk/25.0.8775105 --use_nnapi --android_abi arm64-v8a --android_api 29

./build.sh --allow_running_as_root --parallel --build_shared_lib --android --android_sdk_path /root/android_sdk --android_ndk_path /root/android_sdk/ndk/25.0.8775105 --use_nnapi --android_abi arm64-v8a --android_api 27  --cmake_generator Ninja

```
