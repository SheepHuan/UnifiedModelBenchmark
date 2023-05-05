#!/bin/bash

ROOT_DIR=$(pwd)
ORT_SOURCE_DIR="$ROOT_DIR/3rd-party/onnxruntime"
# 2.编译onnxruntime 1.14.1
# ORT_BUILD_DIR="$ORT_SOURCE_DIR/build" 
# ORT_INSTALL_DIR="$ORT_SOURCE_DIR/install" 
# mkdir -p $ORT_BUILD_DIR
# mkdir -p $ORT_INSTALL_DIR
cd  $ORT_SOURCE_DIR
chmod +x build.sh
./build.sh --allow_running_as_root --config RelWithDebInfo --build_shared_lib --parallel --cmake_generator  Ninja 