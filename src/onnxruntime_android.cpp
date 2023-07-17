#include "core/session/onnxruntime_cxx_api.h"
#include "core/session/onnxruntime_c_api.h"
// #ifdef __ANDROID__
// #include "core/providers/nnapi/nnapi_provider_factory.h"
// #endif
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>
#include <vector>
#include "mutils/profile.hpp"
#include "mutils/timer.hpp"
#include "mutils/args.hpp"
#include <float.h>

void print_args()
{
    LOG(INFO) <<"=================================\t"<<"Args Info"<<"\t=================================";
    std::string model_path = FLAGS_model;
    std::string backend = FLAGS_backend;
    bool enable_op_profiling = FLAGS_enable_op_profiling;
    std::string result_prefix = FLAGS_prefix;
    int num_threads = FLAGS_num_threads;
    int nums_warmup = FLAGS_num_warmup;
    int num_runs = FLAGS_num_runs;

    LOG(INFO) << "model path: " << model_path;
    LOG(INFO) << "backend: " << backend;
    LOG(INFO) << "cpu threads: " << num_threads;
    LOG(INFO) << "nums_warmup: " << nums_warmup;
    LOG(INFO) << "num_runs: " << num_runs;
}

int run(Ort::Session &session, int nums_warmup, int num_runs)
{
    LOG(INFO) <<"=================================\t"<<"Runtime Info"<<"\t=================================";
    std::vector<Ort::AllocatedStringPtr> ptrs;
    std::vector<Ort::Value> input_tensors;
    std::vector<std::string> input_names;
    std::vector<std::string> output_names;

    size_t input_count = session.GetInputCount();
    size_t output_count = session.GetOutputCount();
    auto mem_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
    Ort::AllocatorWithDefaultOptions allocator;
    for (size_t i = 0; i < input_count; i++)
    {
        std::string input_info = "input: ";

        // 获取输入名字
        Ort::AllocatedStringPtr input_name_ptr = session.GetInputNameAllocated(i, allocator);
        std::string input_name = std::string(input_name_ptr.get());
        input_names.push_back(input_name);
        input_info += input_names[i];
        input_info += ", dim: [";
        // 获取输入维度
        std::vector<int64_t> input_dim = session.GetInputTypeInfo(i).GetTensorTypeAndShapeInfo().GetShape();

        size_t size = 1;
        for (size_t j = 0; j < input_dim.size(); j++)
        {
            if (input_dim[j] == -1)
            {
                input_dim[j] = 1;
            }
            if (j != 0)
                input_info += ",";
            input_info += std::to_string(input_dim[j]);
            size *= input_dim[j];
        }
        input_info += "]";
        LOG(INFO) << i << " " << input_info;

        float *float32_data = (float *)malloc(sizeof(float) * size);
        input_tensors.push_back(
            Ort::Value::CreateTensor<float>(
                mem_info, float32_data, size, input_dim.data(), input_dim.size()));
    }

    for (size_t i = 0; i < output_count; i++)
    {
        std::string output_info = "output: ";
        Ort::AllocatedStringPtr output_name_ptr = session.GetOutputNameAllocated(i, allocator);
        std::string output_name = std::string(output_name_ptr.get());
        output_names.push_back(output_name);
        output_info += output_name;

        output_info += ", dim: [";
        // 获取输入维度
        std::vector<int64_t> output_dim = session.GetOutputTypeInfo(i).GetTensorTypeAndShapeInfo().GetShape();
        size_t size = 1;
        for (size_t j = 0; j < output_dim.size(); j++)
        {
            if (output_dim[j] == -1)
            {
                output_dim[j] = 1;
            }
            if (j != 0)
                output_info += ",";
            output_info += std::to_string(output_dim[j]);
            size *= output_dim[j];
        }
        output_info += "]";
        LOG(INFO) << i << " " << output_info;
    }

    double warmup_time = 0;
    MyTimer timer = MyTimer();
    for (int i = 0; i < nums_warmup; i++)
    {
        std::vector<const char *> input_names_ptr;
        std::vector<const char *> output_names_ptr;

        for (size_t i = 0; i < input_count; i++)
        {
            input_names_ptr.push_back(input_names[i].c_str());
        }
        for (size_t i = 0; i < output_count; i++)
        {
            output_names_ptr.push_back(output_names[i].c_str());
        }
        timer.start();
        auto output_tensors = session.Run(Ort::RunOptions{nullptr}, input_names_ptr.data(), input_tensors.data(), input_count, output_names_ptr.data(), output_count);
        timer.end();
        warmup_time = warmup_time + timer.get_time();
    }
     double latency_avg = 0, latency_std = 0,latency_max=DBL_MIN,latency_min=DBL_MAX;
    std::vector<double> latency_per_rounds;
    for (int i = 0; i < num_runs; i++)
    {
        std::vector<const char *> input_names_ptr;
        std::vector<const char *> output_names_ptr;
        for (size_t i = 0; i < input_count; i++)
        {
            input_names_ptr.push_back(input_names[i].c_str());
        }
        for (size_t i = 0; i < output_count; i++)
        {
            output_names_ptr.push_back(output_names[i].c_str());
        }
        timer.start();
        auto output_tensors = session.Run(Ort::RunOptions{nullptr}, input_names_ptr.data(), input_tensors.data(), input_count, output_names_ptr.data(), output_count);
        timer.end();
        latency_per_rounds.push_back(timer.get_time());
        //  latency_max = timer.get_time() > latency_max ? timer.get_time() : latency_max;
        // latency_min = timer.get_time() < latency_min ? timer.get_time() : latency_min;
    }
    profile_latency(latency_per_rounds, latency_per_rounds.size(),latency_min,latency_max, latency_avg, latency_std);
    LOG(INFO) << "warmup: " << nums_warmup << " rounds, avg time: " << warmup_time * 1.0 / nums_warmup << " us";
    LOG(INFO) << "run: " << num_runs << " rounds, min: "<<latency_min<<" us, max: "<<latency_max <<" us, avg: " << latency_avg << " us, std: " << latency_std << " us";
    return 0;
}

int main(int argc, char **argv)
{
    // 解析命令行参数
    init_env(argc,argv);
    std::string model_path = FLAGS_model;
    std::string backend = FLAGS_backend;
    bool enable_op_profiling = FLAGS_enable_op_profiling;
    std::string result_prefix = FLAGS_prefix;
    int num_threads = FLAGS_num_threads;
    int nums_warmup = FLAGS_num_warmup;
    int num_runs = FLAGS_num_runs;

    print_args();
    // 初始化推理会话
    Ort::Env env = Ort::Env{ORT_LOGGING_LEVEL_ERROR, "Default"};
    Ort::SessionOptions session_options;
    if (enable_op_profiling)
        session_options.EnableProfiling(result_prefix.c_str());
    session_options.SetIntraOpNumThreads(num_threads);
    session_options.SetExecutionMode(ExecutionMode::ORT_SEQUENTIAL);
    session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);

    Ort::Session session{env, model_path.c_str(), session_options}; // CPU
    if (backend == "arm")
    {
        run(session, nums_warmup, num_runs);
    }
    else if (backend == "nnapi")
    {
        LOG(WARNING) << "flags for ort nnapi ep: NNAPI_FLAG_CPU_DISABLED";
         /*
            这里可以设置文档地址(https://onnxruntime.ai/docs/execution-providers/NNAPI-ExecutionProvider.html)，例如下:
            nnapi_flags |= NNAPI_FLAG_USE_FP16;
            */
        // #ifdef __ANDROID__
        //         uint32_t nnapi_flags = 0;
        //         nnapi_flags |= NNAPI_FLAG_CPU_DISABLED;
        //         // nnapi_flags |= NNAPI_FLAG_USE_FP16;
        //         Ort::ThrowOnError(OrtSessionOptionsAppendExecutionProvider_Nnapi(session_options, nnapi_flags));
        // #else
        //         LOG(ERROR)<<"Current Complier is not for android!";
        // #endif
        run(session, nums_warmup, num_runs);
    }
    else
    {
        LOG(ERROR) << "unkown backend for onnxruntime: " << backend;
    }
    return 0;


}