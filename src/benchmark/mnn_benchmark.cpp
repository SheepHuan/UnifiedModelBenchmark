#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>
#include <vector>
#include "mutils/profiler.hpp"
#include "mutils/init.hpp"
#include "MNN/Interpreter.hpp"

std::string str_shape(std::vector<int> shape)
{
    std::stringstream ss;
    for (auto dim : shape)
    {
        ss << std::to_string(dim) << ",";
    }
    return ss.str();
}

void benchmark(std::shared_ptr<MNN::Interpreter> net, MNN::Session *session, int num_warmup, int num_run)
{
    MNN::TensorCallBackWithInfo before_callback = [&](const std::vector<MNN::Tensor *> &ntensors,
                                                      const MNN::OperatorInfo *info)
    {
        // if (opTypes.find(info->name()) == opTypes.end())
        // {
        //     opTypes.insert(std::make_pair(info->name(), info->type()));
        // }
        // opBegin = getTimeInUs();
        // if (opTimes.find(info->name()) == opTimes.end())
        // {
        //     opTimes.insert(std::make_pair(info->name(), std::make_pair(0.0f, info->flops())));
        // }
        return true;
    };

    MNN::TensorCallBackWithInfo after_callback = [&](const std::vector<MNN::Tensor *> &ntensors,
                                                     const MNN::OperatorInfo *info)
    {
        // auto opEnd = getTimeInUs();
        // float cost = (float)(opEnd - opBegin) / 1000.0f;

        // opTimes[info->name()].first += cost;
        return true;
    };

    // 第一步是填充数据
    std::map<std::string, MNN::Tensor *> inputs_map = net->getSessionInputAll(session);
    for (auto pair : inputs_map)
    {
        auto name = pair.first;
        auto tensor = pair.second;

        std::vector<int> shape = tensor->shape();
        // NCHW的Tensor data layout
        MNN::Tensor *tmp_tensor = new MNN::Tensor(tensor, MNN::Tensor::CAFFE);
        memset(tmp_tensor->host<float>(), 1.0f, tmp_tensor->size());
        tensor->copyFromHostTensor(tmp_tensor);
        delete tmp_tensor;
        // LOG(INFO) << "name: " << name << ", shape: " << str_shape(shape) << ", filled by 1.0f";
    }
    huan::benchmark::MyProfiler profiler = huan::benchmark::MyProfiler();
    std::vector<double> warmup_lat_data, run_lat_data;
    for (auto i = 0; i < num_warmup; i++)
    {
        profiler.start();
        MNN::ErrorCode code = net->runSessionWithCallBackInfo(session, before_callback, after_callback, true);
        // MNN::ErrorCode code = net->runSession(session);
        profiler.end();
        warmup_lat_data.push_back(profiler.get_time());
        net->updateCacheFile(session);
    }
    for (auto i = 0; i < num_run; i++)
    {
        profiler.start();
        MNN::ErrorCode code = net->runSessionWithCallBackInfo(session, before_callback, after_callback, true);
        // MNN::ErrorCode code = net->runSession(session);
        profiler.end();
        run_lat_data.push_back(profiler.get_time());
    }
    LOG(INFO) << "WARMUP\t" << huan::benchmark::show_latency_metric(warmup_lat_data);
    LOG(INFO) << "RUN\t" << huan::benchmark::show_latency_metric(run_lat_data);
}

int main(int argc, char **argv)
{
    init_env(argc, argv);
    std::string model_path = FLAGS_model;
    std::string backend = FLAGS_backend;
    int num_threads = FLAGS_num_threads;
    int num_warmup = FLAGS_num_warmup;
    int num_runs = FLAGS_num_runs;

    LOG(INFO) << "=================================\t"
              << "MNN Args Info"
              << "\t=================================";
    LOG(INFO) << "model path: " << model_path;
    LOG(INFO) << "backend: " << backend;
    LOG(INFO) << "cpu threads: " << num_threads;
    LOG(INFO) << "num_warmup: " << num_warmup;
    LOG(INFO) << "num_runs: " << num_runs;


    std::shared_ptr<MNN::Interpreter> interpreter = std::shared_ptr<MNN::Interpreter>(MNN::Interpreter::createFromFile(model_path.c_str()));
    if (nullptr == interpreter)
    {
        return 0;
    }
    //  GPU cache
    interpreter->setCacheFile(".tempcache");
    MNN::ScheduleConfig conf;
    MNN::BackendConfig backendConfig;

    backendConfig.precision = static_cast<MNN::BackendConfig::PrecisionMode>(MNN::BackendConfig::PrecisionMode::Precision_Normal);
    backendConfig.memory = static_cast<MNN::BackendConfig::MemoryMode>(MNN::BackendConfig::MemoryMode::Memory_Normal);
    backendConfig.power = static_cast<MNN::BackendConfig::PowerMode>(MNN::BackendConfig::PowerMode::Power_Normal);
    conf.backendConfig = &backendConfig;
    conf.backupType = MNNForwardType::MNN_FORWARD_CPU;
#ifdef __linux__
    if (backend == "x64")
    {
        conf.numThread = num_threads;
        conf.backupType = MNNForwardType::MNN_FORWARD_CPU;
    }
#endif
#ifdef __ANDROID__
    if (backend == "arm")
    {
        LOG(INFO) << "backend arm, num thread 4";
        conf.numThread = num_threads;
        conf.backupType = MNNForwardType::MNN_FORWARD_CPU;
    }
    else if (backend == "opencl")
    {
        LOG(INFO) << "backend opencl, MNN_GPU_TUNING_WIDE | MNN_GPU_MEMORY_IMAGE";
        // 设置GPU Mode是image or buffer
        conf.mode = MNNGpuMode::MNN_GPU_TUNING_WIDE | MNNGpuMode::MNN_GPU_MEMORY_IMAGE;
        conf.backupType = MNNForwardType::MNN_FORWARD_OPENCL;
    }
    else if (backend == "nnapi")
    {
        conf.backupType = MNNForwardType::MNN_FORWARD_NN;
    }
#endif
    MNN::Session *session = interpreter->createSession(conf);
    benchmark(interpreter, session, num_warmup, num_runs);
}
