#include "mutils/timer.hpp"
#include "mutils/profile.hpp"
#include "mutils/tensor.hpp"
#include "mutils/args.hpp"
#include <sstream>
#include <iostream>
#include "paddle_api.h"         // NOLINT
// #include "paddle_use_passes.h"  // NOLINT
// #include "paddle_use_kernels.h"  // NOLINT
// #include "paddle_use_ops.h"      // NOLINT

using namespace paddle::lite_api;
// https://github.com/PaddlePaddle/Paddle-Lite/blob/develop/lite/api/tools/benchmark/benchmark.h
// https://github.com/PaddlePaddle/Paddle-Lite/blob/develop/lite/demo/cxx/mobile_full/mobilenetv1_full_api.cc





void run(std::shared_ptr<PaddlePredictor> predictor, huan::benchmark::MTensorDict input_tensors_info, int num_warmup, int num_runs)
{

    std::vector<std::string> input_names = predictor->GetInputNames();
    int input_count = 0, output_count = 0;
    assert(input_names.size() == input_tensors_info.inputs_info.size());
    for (int i = 0; i < input_names.size(); i++)
    {
        input_count++;
        std::unique_ptr<Tensor> input_tensor(std::move(predictor->GetInput(0)));
        LOG(INFO) << "input tensor: " << input_names[i];
        LOG(INFO) << "input tensor: " << input_tensors_info.inputs_info[i].name;
        shape_t shape = input_tensors_info.query_pd_shape_info(input_names[i]); //input_info[input_names[i]];
        LOG(INFO) << "input tensor: " << shape.size();
        input_tensor->Resize(shape);
        auto *data = input_tensor->mutable_data<float>();
        memset(data, 1, huan::benchmark::shape_production(input_tensor->shape()) * sizeof(float));

    }
    MyTimer timer = MyTimer();
    double latency_avg = 0, latency_std = 0, latency_max = DBL_MIN, latency_min = DBL_MAX;
    std::vector<double> latency_per_rounds;

    double warmup_time=0;
    for (int i = 0; i < num_warmup; i++)
    {
        timer.start();
        predictor->Run();
        timer.end();
       warmup_time+=timer.get_time();
    }

    for (int i = 0; i < num_runs; i++)
    {
        timer.start();
        predictor->Run();
        timer.end();
        latency_per_rounds.push_back(timer.get_time());
    
    }
    profile_latency(latency_per_rounds, latency_per_rounds.size(),latency_min,latency_max, latency_avg, latency_std);
    LOG(INFO) << "warmup: " << num_warmup << " rounds, avg time: " << warmup_time * 1.0 / num_warmup << " us";
    LOG(INFO) << "run: " << num_runs << " rounds, min: " << latency_min << " us, max: " << latency_max << " us, avg : " << latency_avg << " us, std: " << latency_std << " us";
}

int main(int argc, char **argv)
{

    // 解析命令行参数
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    std::string model_path = FLAGS_model;
    std::string param_path = FLAGS_param;
    std::string optimized_model_path = FLAGS_optimized_model_path;
    int cpu_power_mode = FLAGS_cpu_power_mode;
    int num_threads = FLAGS_num_threads;
    num_threads = std::min(num_threads, 8);
    std::string backend = FLAGS_backend;
    int num_warmup = FLAGS_num_warmup;
    int num_runs = FLAGS_num_runs;
    std::string str_input_info = FLAGS_input_info;
    
    
    std::vector<huan::benchmark::MTensorInfo> input_tensors_info;
    huan::benchmark::parse_tensor_info(str_input_info,input_tensors_info);
    huan::benchmark::MTensorDict input_tensors_dict(input_tensors_info);
    
    // // 打印参数
    LOG(INFO) << "=================================\t"
              << "Args Info"
              << "\t=================================";
    LOG(INFO) << "model path: " << model_path;
    LOG(INFO) << "param path: " << param_path;
    LOG(INFO) << "optimized model dir: " << optimized_model_path;
    LOG(INFO) << "backend: " << backend;
    LOG(INFO) << "cpu threads: " << num_threads;
    LOG(INFO) << "nums_warmup: " << num_warmup;
    LOG(INFO) << "num_runs: " << num_runs;
    LOG(INFO) << "input_info: " << str_input_info;

    CxxConfig config;
    config.set_model_file(model_path);
    config.set_param_file(param_path);
    config.set_threads(num_threads);
    config.set_power_mode(static_cast<PowerMode>(cpu_power_mode));

    std::vector<Place> valid_places;
    if (backend == "opencl")
    {
        valid_places.emplace_back(
            Place{TARGET(kOpenCL), PRECISION(kFP16), DATALAYOUT(kImageDefault)});
        valid_places.emplace_back(
            Place{TARGET(kOpenCL), PRECISION(kFP16), DATALAYOUT(kImageFolder)});
        valid_places.emplace_back(
            Place{TARGET(kOpenCL), PRECISION(kFloat), DATALAYOUT(kNCHW)});
        valid_places.emplace_back(
            Place{TARGET(kOpenCL), PRECISION(kAny), DATALAYOUT(kImageDefault)});
        valid_places.emplace_back(
            Place{TARGET(kOpenCL), PRECISION(kAny), DATALAYOUT(kImageFolder)});
        valid_places.emplace_back(
            Place{TARGET(kOpenCL), PRECISION(kAny), DATALAYOUT(kNCHW)});
        valid_places.emplace_back(
            Place{TARGET(kOpenCL), PRECISION(kInt32), DATALAYOUT(kNCHW)});
        valid_places.emplace_back(Place{TARGET(kARM)});
    }
    else if (backend == "arm")
    {
        valid_places.emplace_back(Place{TARGET(kARM), PRECISION(kFloat)});
    }
    else
    {
        LOG(ERROR) << "Don't support backend: " << backend << ", just support arm or opencl.";
        return -1;
    }
    config.set_valid_places(valid_places);

    // 2. Create PaddlePredictor by CxxConfig
    std::shared_ptr<PaddlePredictor> predictor =
        CreatePaddlePredictor<CxxConfig>(config);

    // 3. Save the optimized model
    predictor->SaveOptimizedModel(optimized_model_path,
                                  LiteModelType::kNaiveBuffer);
    
    run(predictor, input_tensors_dict, num_warmup, num_runs);
    return 0;
}