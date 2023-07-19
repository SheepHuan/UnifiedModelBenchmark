#!/bin/bash
export ANDROID_NDK="/root/android_sdk/ndk/25.0.8775105"
export PATH=$PATH:/root/cmake-3.27.0-rc4-linux-x86_64/bin

# git clone --recursive -b v1.15.1 https://github.com/microsoft/onnxruntime.git 

cd onnxruntime

# build for android
./build.sh --config Release \
--allow_running_as_root \
--parallel \
--build_shared_lib \
--android  \
--use_nnapi \
--android_sdk_path /root/android_sdk \
--android_ndk_path /root/android_sdk/ndk/25.0.8775105 \
--android_abi arm64-v8a \
--android_api 27 \
--cmake_generator Ninja

# libonnxruntime.so:     file format elf64-little

# Program Header:
#     PHDR off    0x0000000000000040 vaddr 0x0000000000000040 paddr 0x0000000000000040 align 2**3
#          filesz 0x00000000000001f8 memsz 0x00000000000001f8 flags r--
#     LOAD off    0x0000000000000000 vaddr 0x0000000000000000 paddr 0x0000000000000000 align 2**12
#          filesz 0x0000000000cc6380 memsz 0x0000000000cc6380 flags r-x
#     LOAD off    0x0000000000cc6380 vaddr 0x0000000000cc7380 paddr 0x0000000000cc7380 align 2**12
#          filesz 0x0000000000053310 memsz 0x0000000000053310 flags rw-
#     LOAD off    0x0000000000d19690 vaddr 0x0000000000d1b690 paddr 0x0000000000d1b690 align 2**12
#          filesz 0x0000000000000ac0 memsz 0x0000000000005d30 flags rw-
#  DYNAMIC off    0x0000000000d11f18 vaddr 0x0000000000d12f18 paddr 0x0000000000d12f18 align 2**3
#          filesz 0x00000000000001e0 memsz 0x00000000000001e0 flags rw-
#    RELRO off    0x0000000000cc6380 vaddr 0x0000000000cc7380 paddr 0x0000000000cc7380 align 2**0
#          filesz 0x0000000000053310 memsz 0x0000000000053c80 flags r--
# EH_FRAME off    0x00000000002758f8 vaddr 0x00000000002758f8 paddr 0x00000000002758f8 align 2**2
#          filesz 0x0000000000033e5c memsz 0x0000000000033e5c flags r--
#    STACK off    0x0000000000000000 vaddr 0x0000000000000000 paddr 0x0000000000000000 align 2**0
#          filesz 0x0000000000000000 memsz 0x0000000000000000 flags rw-
#     NOTE off    0x0000000000000238 vaddr 0x0000000000000238 paddr 0x0000000000000238 align 2**2
#          filesz 0x00000000000000bc memsz 0x00000000000000bc flags r--

# Dynamic Section:
#   RUNPATH              $ORIGIN
#   NEEDED               libdl.so
#   NEEDED               liblog.so
#   NEEDED               libm.so
#   NEEDED               libc.so
#   FLAGS                0x0000000000000008
#   FLAGS_1              0x0000000000000001
#   RELA                 0x0000000000002178
#   RELASZ               0x00000000000e2098
#   RELAENT              0x0000000000000018
#   RELACOUNT            0x00000000000096ab
#   JMPREL               0x00000000000e4210
#   PLTRELSZ             0x00000000000012c0
#   PLTGOT               0x0000000000d1a038
#   PLTREL               0x0000000000000007
#   SYMTAB               0x00000000000002f8
#   SYMENT               0x0000000000000018
#   STRTAB               0x00000000000018b8
#   STRSZ                0x00000000000008be
#   GNU_HASH             0x0000000000001890
#   INIT_ARRAY           0x0000000000d12ea0
#   INIT_ARRAYSZ         0x0000000000000078
#   FINI_ARRAY           0x0000000000d12e90
#   FINI_ARRAYSZ         0x0000000000000010
#   VERSYM               0x0000000000001648
#   VERDEF               0x00000000000017e4
#   VERDEFNUM            0x0000000000000002
#   VERNEED              0x000000000000181c
#   VERNEEDNUM           0x0000000000000003

# Version definitions:
# 1 0x01 0x0fee26af libonnxruntime.so
# 2 0x00 0x027be181 VERS_1.15.1

# Version References:
#   required from libdl.so:
#     0x00050d63 0x00 04 LIBC
#   required from libm.so:
#     0x00050d63 0x00 05 LIBC
#   required from libc.so:
#     0x00050d63 0x00 03 LIBC
#     0x050d693e 0x00 06 LIBC_N


# build for linux x86
./build.sh --config Release \
--allow_running_as_root \
--parallel \
--build_shared_lib