#include "mutils/init.hpp"
DEFINE_string(model, "", "paddlelite/ncnn model path,onnxrumtime graph path");
DEFINE_string(param, "", "paddlelite/ncnn param path");
DEFINE_string(backend, "arm", R"(paddlelite: arm,opencl"
                              "onnxruntime: arm,nnapi"
                              "ncnn: arm,vulkan"
                              "mnn: arm,x64,nnapi,opencl)");
DEFINE_int32(num_warmup, 3, "num warmup");
DEFINE_int32(num_runs, 10, "num runs");
DEFINE_int32(num_threads, 4, "num threads");
DEFINE_string(input_info, "", "input info");

// FOR PADDLELITE
DEFINE_string(optimized_model_path, "", "Optimized model dir.");
DEFINE_int32(cpu_power_mode, 0, R"(power mode: "
                                "0 for POWER_HIGH;"
                                "1 for POWER_LOW;"
                                "2 for POWER_FULL;"
                                "3 for NO_BIND)");

// FOR ONNXRUNTIME
DEFINE_bool(enable_op_profiling, false, "enable_op_profiling");
DEFINE_string(prefix, "", "result");

// FOR PROFILE & DEBUG
DEFINE_int32(monitor_interval, 3000, "default 3000, unit us=1e-6s");
DEFINE_string(android_current_file, "/sys/class/power_supply/battery/current_now", "android current file, unit uA=1e-6A");
DEFINE_string(android_voltage_file, "/sys/class/power_supply/battery/voltage_now", "android voltage file, unit uV=1e-6V");

void init_env(int argc, char **argv)
{
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::GLOG_INFO);
}