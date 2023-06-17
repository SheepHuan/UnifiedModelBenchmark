##
[Linux x86 环境下编译适用于 Android 的库](https://www.paddlepaddle.org.cn/lite/develop/source_compile/linux_x86_compile_android.html)
```bash
git clone https://github.com/PaddlePaddle/Paddle-Lite.git --branch release/v2.13

```


```bash
apt install curl -y
# 1. 设置NDK目录
# https://dl.google.com/android/repository/android-ndk-r25c-linux.zip
# 因为sdk里面的NDK命名时版本号,paddlite的脚本不识别
export NDK_ROOT=/root/tmp/android-ndk-r25c

# 2. 切入分支 2.13
git checkout release/v2.12
# 3. (可选) 删除 third-party 目录，编译脚本会自动从国内 CDN 下载第三方库文件
rm -rf third-party
# 4. 编译CPU+OPENCL库 ,--with_benchmark
bash lite/tools/build_android.sh \
--arch=armv8 \
--toolchain=clang \
--android_stl=c++_shared \
--with_java=OFF \
--with_profile=ON \
--android_api_level=27 \
--with_arm82_fp16=ON \
--with_extra=ON \
--with_opencl=ON

bash lite/tools/build_android.sh \
--arch=armv8 \
--toolchain=clang \
--android_stl=c++_shared \
--with_java=OFF \
--with_profile=ON \
--android_api_level=27 \
--with_arm82_fp16=ON \
--with_extra=ON \
--with_opencl=ON full_publish

bash lite/tools/build_android.sh \
--arch=armv8 \
--toolchain=clang \
--android_stl=c++_shared \
--with_profile=ON \
--android_api_level=27 full_publish
```


```text
inference_lite_lib.android.armv8.opencl/
|-- cxx
|   |-- include
|   |   |-- paddle_api.h
|   |   |-- paddle_image_preprocess.h
|   |   |-- paddle_lite_factory_helper.h
|   |   |-- paddle_place.h
|   |   |-- paddle_use_kernels.h
|   |   |-- paddle_use_ops.h
|   |   `-- paddle_use_passes.h
|   `-- lib
|       `-- libpaddle_light_api_shared.so
|-- demo
|   `-- cxx
|       |-- Makefile.def
|       |-- README.md
|       |-- lac_demo
|       |   |-- Makefile
|       |   |-- lac.cc
|       |   |-- lac.h
|       |   |-- lac_demo.cc
|       |   |-- lac_util.cc
|       |   |-- lac_util.h
|       |   |-- prepare.sh
|       |   |-- readme.md
|       |   `-- run.sh
|       |-- mask_detection
|       |   |-- Makefile
|       |   |-- mask_detection.cc
|       |   |-- prepare.sh
|       |   `-- run.sh
|       |-- mobile_classify
|       |   |-- Makefile
|       |   `-- mobile_classify.cc
|       |-- mobile_light
|       |   |-- Makefile
|       |   `-- mobilenetv1_light_api.cc
|       |-- mobilenetv1_light_from_buffer
|       |   |-- Makefile
|       |   `-- mobilenetv1_light_api.cc
|       |-- ssd_detection
|       |   |-- Makefile
|       |   `-- ssd_detection.cc
|       |-- test_cv
|       |   |-- Makefile
|       |   |-- README.md
|       |   |-- test_img_prepross.cc
|       |   `-- test_model_cv.cc
|       `-- yolov3_detection
|           |-- Makefile
|           `-- yolov3_detection.cc
`-- opencl
    `-- cl_kernel
        |-- buffer
        |   |-- activation_kernel.cl
        |   |-- concat_kernel.cl
        |   |-- depthwise_conv2d_kernel.cl
        |   |-- elementwise_add_kernel.cl
        |   |-- fc_kernel.cl
        |   |-- im2col_kernel.cl
        |   |-- mat_mul_kernel.cl
        |   |-- pool_kernel.cl
        |   |-- pow_kernel.cl
        |   |-- precision_cast_kernel.cl
        |   |-- scale_kernel.cl
        |   |-- slice_kernel.cl
        |   |-- transpose_kernel.cl
        |   `-- yolo_box_kernel.cl
        |-- cl_common.h
        `-- image
            |-- activation_kernel.cl
            |-- argmax_kernel.cl
            |-- batch_norm_kernel.cl
            |-- bilinear_interp_kernel.cl
            |-- box_coder_kernel.cl
            |-- channel_add_kernel.cl
            |-- clip_kernel.cl
            |-- concat_default_kernel.cl
            |-- concat_kernel.cl
            |-- conv2d_1x1_default_kernel.cl
            |-- conv2d_1x1_default_mali_kernel.cl
            |-- conv2d_1x1_opt_kernel.cl
            |-- conv2d_3x3_kernel.cl
            |-- conv2d_common_kernel.cl
            |-- conv2d_nxn_kernel.cl
            |-- conv2d_opt_adreno_kernel.cl
            |-- conv2d_opt_mali_kernel.cl
            |-- conv2d_transpose_kernel.cl
            |-- conv2d_winograd_3x3s1_kernel.cl
            |-- depthwise_conv2d_basic_kernel.cl
            |-- depthwise_conv2d_kernel.cl
            |-- depthwise_conv2d_transpose_kernel.cl
            |-- dropout_kernel.cl
            |-- elementwise_add_kernel.cl
            |-- elementwise_broadcast_kernel.cl
            |-- elementwise_kernel.cl
            |-- elementwise_mul_kernel.cl
            |-- elementwise_sub_kernel.cl
            |-- expand_kernel.cl
            |-- fc_kernel.cl
            |-- gather_kernel.cl
            |-- greater_than_kernel.cl
            |-- grid_sampler_kernel.cl
            |-- instance_norm_kernel.cl
            |-- layer_norm_kernel.cl
            |-- layout_kernel.cl
            |-- lrn_kernel.cl
            |-- matmul_kernel.cl
            |-- matmul_opt_kernel.cl
            |-- matmul_unpersistable_y_kernel.cl
            |-- matmul_xtranspose_kernel.cl
            |-- max_kernel.cl
            |-- nearest_interp_kernel.cl
            |-- pad2d_kernel.cl
            |-- pixel_shuffle_kernel.cl
            |-- pool_deprecated_kernel.cl
            |-- pool_kernel.cl
            |-- reduce_kernel.cl
            |-- reshape_kernel.cl
            |-- scale_kernel.cl
            |-- shuffle_channel_kernel.cl
            |-- slice_kernel.cl
            |-- softmax_kernel.cl
            |-- split_kernel.cl
            |-- transpose_kernel.cl
            |-- trigonometric_kernel.cl
```


```C++
typedef enum {
  LITE_POWER_HIGH = 0,
  LITE_POWER_LOW = 1,
  LITE_POWER_FULL = 2,
  LITE_POWER_NO_BIND = 3,
  LITE_POWER_RAND_HIGH = 4,
  LITE_POWER_RAND_LOW = 5
} PowerMode;


```