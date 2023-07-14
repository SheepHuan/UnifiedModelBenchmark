#!/bin/bash

apt install build-essential -y

# cmake
cd tmp/
wget https://github.com/Kitware/CMake/releases/download/v3.27.0-rc4/cmake-3.27.0-rc4-linux-x86_64.tar.gz
tar -zxvf cmake-3.27.0-rc4-linux-x86_64.tar.gz
mv cmake-3.27.0-rc4-linux-x86_64 /root
# TODO add it to path