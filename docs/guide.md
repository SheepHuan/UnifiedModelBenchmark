## 项目整体依赖情况
1. gflags, 实现设置命令行参数.
2. glog, 实现统一的日志打印和存储.
3. onnxruntime, onnxruntime的推理库,支持的算子最多,但是cpu性能并非最优.
4. ncnn, ncnn的推理库,主要支持cpu和vulkan gpu的推理运算,cpu性能较优秀,gpu性能一般.
5. mnn,
6. paddlelite,


## 项目构建顺序



## 项目构建的指令


## 项目的命令行参数

### onnxruntime
- `model`, 模型图的路径.
- `backend`, ort的推理后端,默认`arm`,可选`arm`,`nnapi`.
- `num_threads`, ort的后端为`arm`的时候，可以选择CPU推理数量`1,..,4,..,8`
- `nums_warmup`, 热身次数，不参与计时.
- `num_runs`, 推理次数.
- `enable_op_profiling`, 是否打开op profiling,这将会保存文件到当前目录,默认是`false`
- `prefix`, 当`enable_op_profiling=true`时，可以设置`prefix`作为文件前缀，改变保存目录。

### paddlelite benchmark

- `model`, 模型图`inference.pdmodel`的路径.
- `param`, 模型参数`inference.pdiparams`的路径.
- `backend`, ort的推理后端,默认`arm`,可选`arm`,`opencl`.
- `num_threads`, ort的后端为`arm`的时候，可以选择CPU推理数量`1,..,4,..,8`
- `nums_warmup`, 热身次数，不参与计时.
- `num_runs`, 推理次数.
- `input_info`, 输入张量的信息格式为"[input1,1x3x128x128,float32],[input2,1x4x56x56,float32]".支持多输入

### ncnn benchmark

- `model`, 模型图`*.bin`的路径.
- `param`, 模型参数`*.param`的路径.
- `backend`, ort的推理后端,默认`arm`,可选`arm`,`vulkan`.
- `num_threads`, ort的后端为`arm`的时候，可以选择CPU推理数量`1,..,4,..,8`
- `nums_warmup`, 热身次数，不参与计时.
- `num_runs`, 推理次数.
- `input_info`, 输入张量的信息格式为"[input1,1x3x128x128,float32],[input2,1x4x56x56,float32]".支持多输入