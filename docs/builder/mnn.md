
[地址](https://mnn-docs.readthedocs.io/)

## demo

```bash


```


## 编译参数
[编译宏解释](https://github.com/alibaba/MNN/wiki/cmake#%E7%BC%96%E8%AF%91%E5%AE%8F%E4%BB%8B%E7%BB%8D)
### linux
- MNN_BUILD_CONVERTER,编译MNN的转换工具
- MNN_BUILD_QUANTOOLS,编译MNN的量化工具


```bash
mkdir build 
cd build 

cmake -DCMAKE_BUILD_TYPE=Release \
-DMNN_USE_SSE=ON \
-DMNN_BUILD_SHARED_LIBS=ON \
-DMNN_BUILD_CONVERTER=ON \
-DMNN_BUILD_QUANTOOLS=ON \
-DMNN_WIN_RUNTIME_MT=ON \
-DCMAKE_INSTALL_PREFIX=/root/workspace/UnifiedModelBenchmark/3rd-party/MNN/build-linux/install \
-G Ninja \
..


cmake -DCMAKE_BUILD_TYPE=Release \
-DMNN_USE_SSE=ON \
-DMNN_BUILD_SHARED_LIBS=ON \
-DMNN_BUILD_CONVERTER=ON \
-DMNN_BUILD_QUANTOOLS=ON \
-DMNN_WIN_RUNTIME_MT=ON \
-DCMAKE_INSTALL_PREFIX=/workspace/MNN/build/install \
..





# Ninja
cmake --build .
cmake --install . --prefix install/
# make
make -j16

```

### android
- MNN_USE_SYSTEM_LIB,设置为ON，表示使用系统库
- MNN_OPENCL,开启OPENCL GPU
- MNN_NNAPI,开启NNAPI
- MNN_ARM82,开启FP16
- MNN_SUPPORT_BF16, 开启BF16

#### CL


#### HW HIAI

```bash
cd project/android
mkdir build-hw-npu
cd build-hw-npu
export ANDROID_NDK=/root/android_sdk/ndk/25.0.8775105


cmake .. \
-DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
-DCMAKE_BUILD_TYPE=Release \
-DANDROID_ABI="arm64-v8a" \
-DANDROID_STL=c++_static \
-DMNN_USE_LOGCAT=false \
-DMNN_BUILD_BENCHMARK=OFF \
-DMNN_USE_SSE=OFF \
-DMNN_SUPPORT_BF16=OFF \
-DMNN_BUILD_TEST=ON \
-DANDROID_NATIVE_API_LEVEL=android-27  \
-DMNN_NPU=ON \
-DMNN_ARM82=ON \
-DMNN_OPENCL=ON \
-DMNN_OPENMP=ON \
-DMNN_VULKAN=ON \
-DMNN_NNAPI=ON \
-DMNN_WITH_PLUGIN=ON\
-DMNN_BUILD_FOR_ANDROID_COMMAND=true \
-DNATIVE_LIBRARY_OUTPUT=. -DNATIVE_INCLUDE_OUTPUT=. $1 $2 $3

make -j16


```
```bash 
cp ../source/backend/hiai/arm64-v8a/libMNN_NPU.so .
cp ../source/backend/opencl/arm64-v8a/libMNN_CL.so . 
cp ../source/backend/vulkan/arm64-v8a/libMNN_Vulkan.so . 
cp ../express/arm64-v8a/libMNN_Express.so .


patchelf --replace-needed ../libMNN.so libMNN.so libMNN_Express.so
patchelf --replace-needed ../../../arm64-v8a/libMNN.so libMNN.so libMNN_CL.so
patchelf --replace-needed ../../../arm64-v8a/libMNN.so libMNN.so libMNN_Vulkan.so
patchelf --replace-needed ../../../arm64-v8a/libMNN.so libMNN.so libMNN_NPU.so
patchelf --replace-needed ../arm64-v8a/libMNN.so libMNN.so libMNN_Express.so
patchelf --replace-needed arm64-v8a/libMNN.so libMNN.so libplugin_matmul.so
```

## 转换
```bash
./MNNConvert --framework=ONNX  \
--keepInputFormat \
--saveStaticModel \
--modelFile=/root/workspace/UnifiedModelBenchmark/tmp/models/resnet50-opset16.onnx  \
--MNNModel=/root/workspace/UnifiedModelBenchmark/tmp/models/mnn/resnet50.mnn

./MNNDump2Json /root/workspace/UnifiedModelBenchmark/tmp/models/mnn/resnet50.mnn  \
/root/workspace/UnifiedModelBenchmark/tmp/models/mnn/resnet50.json
```

