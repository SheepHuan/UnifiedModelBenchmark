#!/bin/bash
# 确保在根目录执行
# 建议export https_proxy代理
ROOT_DIR=$(pwd)
OPENCV_SOURCE_DIR="$ROOT_DIR/3rd-party/opencv"
ORT_SOURCE_DIR="$ROOT_DIR/3rd-party/onnxruntime"
# echo $ROOT_DIR
# 1.编译opencv 4.7.0
OPENCV_BUILD_DIR="$OPENCV_SOURCE_DIR/build" 
OPENCV_INSTALL_DIR="$OPENCV_BUILD_DIR/install" 
mkdir -p $OPENCV_BUILD_DIR
cd  $OPENCV_BUILD_DIR
rm -rf *
cmake -DBUILD_ANDROID_PROJECTS=OFF \
-DBUILD_DOCS=off \
-DBUILD_FAT_JAVA_LIB=off \
-DBUILD_opencv_calib3d=off \
-DBUILD_opencv_contrib=off \
-DBUILD_opencv_features2d=off \
-DBUILD_opencv_flann=off \
-DBUILD_opencv_gpu=off \
-DBUILD_opencv_java=off \
-DBUILD_opencv_legacy=off \
-DBUILD_opencv_ml=off \
-DBUILD_opencv_nonfree=off \
-DBUILD_opencv_objdetect=off \
-DBUILD_opencv_ocl=off \
-DBUILD_opencv_photo=off \
-DBUILD_opencv_python=off \
-DBUILD_opencv_stitching=off \
-DBUILD_opencv_superres=off \
-DBUILD_opencv_ts=off \
-DCMAKE_BUILD_TYPE=Release  \
-DANDROID_STL=c++_shared \
-DBUILD_SHARED_LIBS=ON \
-DBUILD_opencv_world=off \
-DCMAKE_INSTALL_PREFIX=$OPENCV_INSTALL_DIR ..
make -j8
make install

# # 2.编译onnxruntime 1.14.1
# ORT_BUILD_DIR="$ORT_SOURCE_DIR/build" 
# ORT_INSTALL_DIR="$ORT_BUILD_DIR/install" 
# mkdir -p $ORT_BUILD_DIR
# cd  $ORT_BUILD_DIR
# rm -rf *


