# Benchmark

## 编译指令
```bash
# linux 
cd build
cmake ..
cmake --build . --target main

# linux 交叉编译 android 可执行文件


```

## 如何使用gdb调试
[GDB教程](https://zhuanlan.zhihu.com/p/74897601)
```bash
gdb main
# 运行程序
run
# 带命令行参数运行程序
run --args
# 或者
set --args
run

# 添加断点
## 根据行号设置断点
b main.cpp:9

## 根据函数名设置断点

## 根据条件设置断点

## 跳过多次断电
ignore 1 30
# 查看已设置的断点
info breakpoints

```


## 开启git lfs 下载库


## 代码依赖库
1. onnxruntime@v1.14.1
2. opencv@v4.7.0
3. [gflags@2.2.2](https://github.com/gflags/gflags)


## 参考文章
[1] [C++设计实现日志系统](https://zhuanlan.zhihu.com/p/100082717)
