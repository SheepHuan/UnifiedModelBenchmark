# 如何使用 gdb 调试

[GDB 教程](https://zhuanlan.zhihu.com/p/74897601)

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
