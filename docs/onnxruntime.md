# 编译onnxruntime库 （基于Ubuntu20.04环境编译）

```bash
git clone --recursive https://github.com/microsoft/onnxruntime.git
apt install libssl-dev build-essential
git checkout v1.14.1
```

## Build for Linux [doc](https://onnxruntime.ai/docs/build/inferencing.html)
```bash
export https_proxy="http://172.16.101.68:7890"
# 默认编译amd64的CPU onnx version 1.12.0
./build.sh --config RelWithDebInfo --build_shared_lib --parallel --cmake_generator Ninja 
```



## Build for Android [doc](https://onnxruntime.ai/docs/build/android.html)

### 环境准备
#### 环境
```bash
# 可能需要java环境
apt install openjdk-11-jdk
# 编译需要python3
apt install python3.8
ln -s /usr/bin/python3.8 /usr/bin/python3
# cmake环境
wget https://github.com/Kitware/CMake/releases/download/v3.26.0/cmake-3.26.0-linux-x86_64.sh

# 配置环境变量即可...
```

#### 安装Android SdkManager
1. 进入[Android Studio 网址](https://developer.android.com/studio),下载"Command line tools only"
```bash
wget https://dl.google.com/android/repository/commandlinetools-linux-9477386_latest.zip
unzip commandlinetools-linux-9477386_latest.zip
cp cmdline-tools/ ~/android_sdk



# 安装 android 环境
sdkmanager --install "platforms;android-29"
sdkmanager --install "ndk;25.0.8775105"
```

### 交叉编译


```bash
export https_proxy="http://172.16.101.68:7890"
export http_proxy="http://172.16.101.68:7890"
./build.sh --config Release --android --android_sdk_path /root/android_sdk --android_ndk_path /root/android_sdk/ndk/25.0.8775105 --use_nnapi --build_shared_lib --android_abi arm64-v8a --android_api 29 --cmake_generator Ninja 

./build.sh --config RelWithDebInfo --android --android_sdk_path /root/android_sdk --android_ndk_path /root/android_sdk/ndk/25.0.8775105 --use_nnapi  --build_shared_lib --android_abi arm64-v8a --android_api 29 --cmake_generator Ninja 
# 编译完成后取出libonnxruntime.so
```
