#ifndef INIT_HPP
#define INIT_HPP

#include <gflags/gflags.h>
#include <glog/logging.h>
DECLARE_string(model);
DECLARE_string(param);
DECLARE_string(backend);
DECLARE_int32(num_warmup);
DECLARE_int32(num_runs);
DECLARE_int32(num_threads);
DECLARE_string(input_info);

//FOR PADDLELITE
DECLARE_string(optimized_model_path);
DECLARE_int32(cpu_power_mode);

// FOR ONNXRUNTIME
DECLARE_bool(enable_op_profiling);
DECLARE_string(prefix);

// FOR MNN
DECLARE_string(cl_mem_type);
DECLARE_int32(mnn_precision);
// DECLARE_string(mnn_power_mode);

// FOR PROFILE & DEBUG
DECLARE_int32(monitor_interval);
DECLARE_string(android_current_file);
DECLARE_string(android_voltage_file);

void init_env(int argc,char **argv);
#endif