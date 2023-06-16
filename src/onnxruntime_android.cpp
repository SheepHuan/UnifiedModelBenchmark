#include "core/session/onnxruntime_cxx_api.h"
#include "core/session/onnxruntime_c_api.h"
#ifdef ANDROID_PLATFORM
#include "providers/nnapi/nnapi_provider_factory.h"
#endif
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>
#include <vector>
#include "mutils/log.hpp"
#include <gflags/gflags.h>
#include "mutils/profile.hpp"
#include "mutils/timer.hpp"
DEFINE_string(graph, "", "onnx model path");
DEFINE_int32(warmup_runs, 3, "warmup_runs");
DEFINE_int32(num_runs, 10, "num_runs");
DEFINE_int32(num_threads, 2, "num_threads");
// DEFINE_bool(use_nnapi, false, "use nnapi");
DEFINE_bool(enable_op_profiling, false, "enable_op_profiling");
DEFINE_string(prefix, "", "result");

int run(Ort::Session &session)
{
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
        std::vector<int64_t> input_dim = session.GetInputTypeInfo(i).GetTensorTypeAndShapeInfo().GetShape();
        int data_type = session.GetInputTypeInfo(i).GetTensorTypeAndShapeInfo().GetElementType();
        session.GetInputTypeInfo(i).GetTensorTypeAndShapeInfo().GetElementType();
        std::cout << "input dim: ";
        size_t size = 1;
        for (size_t j = 0; j < input_dim.size(); j++)
        {
            if (input_dim[j] == -1)
            {
                input_dim[j] = 1;
            }
            std::cout << input_dim[j] << " ";
            size *= input_dim[j];
        }
        std::cout << std::endl;

        // 获取输入
        Ort::AllocatedStringPtr input_name_ptr = session.GetInputNameAllocated(i, allocator);
        std::string input_name = std::string(input_name_ptr.get());
        input_names.push_back(input_name);

        std::cout << "input_name: " << input_names[i] << std::endl;

        float *float32_data = (float *)malloc(sizeof(float) * size);
        input_tensors.push_back(
            Ort::Value::CreateTensor<float>(
                mem_info, float32_data, size, input_dim.data(), input_dim.size()));
    }

    for (size_t i = 0; i < output_count; i++)
    {
        Ort::AllocatedStringPtr output_name_ptr = session.GetOutputNameAllocated(i, allocator);
        std::string output_name = std::string(output_name_ptr.get());
        output_names.push_back(output_name);

        std::cout << "output_name: " << output_names[i] << std::endl;
    }

    int warmup_rounds = FLAGS_warmup_runs;
    int run_rounds = FLAGS_num_runs;
    double warmup_time = 0;
    MyTimer timer = MyTimer();
    for (int i = 0; i < warmup_rounds; i++)
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
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        timer.end();
        warmup_time = warmup_time + timer.get_time();
    }
    double latency_avg = 0, latency_std = 0;
    std::vector<double> latency_per_rounds;
    for (int i = 0; i < run_rounds; i++)
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
    }
    calc_std_deviation(latency_per_rounds, latency_per_rounds.size(), latency_avg, latency_std);
    printf("warmup: %d rounds, avg time: %f ms\nrun: %d rounds, avg time: %f ms, std: %f ms\n", warmup_rounds, warmup_time * 1.0 / warmup_rounds, run_rounds, latency_avg, latency_std);
    return 0;
}

int main(int argc, char **argv)
{
    // 解析命令行参数
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    std::string model_path = FLAGS_graph;
    bool enable_op_profiling = FLAGS_enable_op_profiling;
    std::string result_prefix = FLAGS_prefix;
    int num_threads = FLAGS_num_threads;
    Ort::Env env = Ort::Env{ORT_LOGGING_LEVEL_ERROR, "Default"};
    Ort::SessionOptions session_options;
    if (enable_op_profiling)
        session_options.EnableProfiling(result_prefix.c_str());
    session_options.SetIntraOpNumThreads(num_threads);
    session_options.SetExecutionMode(ExecutionMode::ORT_SEQUENTIAL);
    session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
    // 注册NNApi
    //     if (FLAGS_use_nnapi)
    //     {

    // /*
    // 这里可以设置文档地址(https://onnxruntime.ai/docs/execution-providers/NNAPI-ExecutionProvider.html)，例如下:
    // nnapi_flags |= NNAPI_FLAG_USE_FP16;
    // */
    // #ifdef ANDROID_PLATFORM
    //         uint32_t nnapi_flags = 0;
    //         nnapi_flags |= NNAPI_FLAG_CPU_ONLY;
    //         Ort::ThrowOnError(OrtSessionOptionsAppendExecutionProvider_Nnapi(session_options, nnapi_flags));
    // #else
    //         mlog::error("Current Complier is not for android!");
    // #endif
    //     }

    Ort::Session session{env, model_path.c_str(), session_options}; // CPU

    run(session);
    return 0;
}