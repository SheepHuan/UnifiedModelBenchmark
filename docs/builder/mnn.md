
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

