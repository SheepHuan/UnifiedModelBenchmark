## 环境要求

```bash
apt install curl -y
# 下载NDK
wget https://dl.google.com/android/repository/android-ndk-r25c-linux.zip
# 因为sdk里面的NDK命名时版本号,paddlite的脚本不识别
```

## 编译推理库
[官方文档地址](https://www.paddlepaddle.org.cn/lite/v2.12/source_compile/linux_x86_compile_android.html)
### shared库
```bash
# 设置NDK环境变量
export NDK_ROOT=/root/tmp/android-ndk-r25c
# 下载PaddleLite源代码，并指定分支为v2.13rc
git clone -b v2.13-rc https://github.com/PaddlePaddle/Paddle-Lite.git
cd Paddle-Lite

# (可选) 删除 third-party 目录，编译脚本会自动从国内 CDN 下载第三方库文件
rm -rf third-party

bash lite/tools/build_android.sh \
--arch=armv8 \
--toolchain=clang \
--android_stl=c++_shared \
--with_java=OFF \
--with_profile=ON \
--with_extra=ON \
--android_api_level=21 \
--with_opencl=ON \
full_publish
```
此时编译的文件位置在`build.lite.android.armv8.clang/inference_lite_lib.android.armv8.opencl/`目录下

```text
cxx
|-- include
|   |-- paddle_api.h
|   |-- paddle_image_preprocess.h
|   |-- paddle_lite_factory_helper.h
|   |-- paddle_place.h
|   |-- paddle_use_kernels.h
|   |-- paddle_use_ops.h
|   `-- paddle_use_passes.h
`-- lib
    |-- libpaddle_api_full_bundled.a
    |-- libpaddle_api_light_bundled.a
    |-- libpaddle_full_api_shared.so
    `-- libpaddle_light_api_shared.so

2 directories, 11 files
```
其中的静态库不可用，引用该静态库会报找不到符号引用(Android Log 和 OpenMP)。


### static库