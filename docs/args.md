# 参数
1. graph, 指定手机设备上模型文件路径。
    - paddlelite,这个推理框架有两类型模型,通过设置参数graph_is_dir来判断时文件夹还是文件。
    - onnxruntime,这个框架只有一种onnx文件。参数graph_is_dir设置对他无效。
2. backend, 指定是手机设备上的arm cpu, opencl gpu的两类后端,可选值[arm,opencl]。
3. num_threads, 当后端为arm cpu时，可指定CPU线程数量/核心数量。
4. cpu_power_mode, 当后端为arm cpu时，可指定功耗数量。
5. nums_warmnup, 热身的轮数。
6. nums_run, 正式运行的轮数。
7. input_shape, 目前仅paddlelite需要手动输入张量形状。
