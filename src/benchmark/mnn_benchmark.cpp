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

    std::map<std::string, MNN::Tensor *> outputs_map = net->getSessionOutputAll(session);
    std::vector<std::shared_ptr<MNN::Tensor>> output_tensors_user;
    for (auto pair : outputs_map)
    {
        auto name = pair.first;
        auto tensor = pair.second;
        std::shared_ptr<MNN::Tensor> output_tensor_user(MNN::Tensor::createHostTensorFromDevice(tensor, false));
        output_tensors_user.push_back(output_tensor_user);
    }

    huan::benchmark::MyProfiler profiler = huan::benchmark::MyProfiler();
    std::vector<double> warmup_lat_data, run_lat_data;
    for (auto i = 0; i < num_warmup; i++)
    {
        profiler.start();
        MNN::ErrorCode code = net->runSessionWithCallBackInfo(session, before_callback, after_callback, true);

        for (auto tmp : output_tensors_user)
        {
            tmp->wait(MNN::Tensor::MAP_TENSOR_READ, true);
        }
        profiler.end();
        warmup_lat_data.push_back(profiler.get_time());

        net->updateCacheFile(session);
    }

    for (auto i = 0; i < num_run; i++)
    {
        profiler.start();
        MNN::ErrorCode code = net->runSessionWithCallBackInfo(session, before_callback, after_callback, true);
        for (auto tmp : output_tensors_user)
        {
            tmp->wait(MNN::Tensor::MAP_TENSOR_READ, true);
        }
        profiler.end();
        run_lat_data.push_back(profiler.get_time());

        net->updateCacheFile(session);
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

    std::string cl_mem_type = FLAGS_cl_mem_type;
    int precision = FLAGS_mnn_precision;

    LOG(INFO) << "=================================\t"
              << "MNN Args Info"
              << "\t=================================";
    LOG(INFO) << "model path: " << model_path;
    LOG(INFO) << "backend: " << backend;
    LOG(INFO) << "cpu threads: " << num_threads;
    LOG(INFO) << "num_warmup: " << num_warmup;
    LOG(INFO) << "num_runs: " << num_runs;
    LOG(INFO) << "cl_mem_type: " << cl_mem_type;
    LOG(INFO) << "precision: " << precision;

    std::shared_ptr<MNN::Interpreter> interpreter = std::shared_ptr<MNN::Interpreter>(MNN::Interpreter::createFromFile(model_path.c_str()));
    if (nullptr == interpreter)
    {
        return 0;
    }
    //  GPU cache
    interpreter->setCacheFile("/data/local/tmp/mnn.tempcache");
    MNN::ScheduleConfig conf;
    MNN::BackendConfig backendConfig;

    switch (precision)
    {
    case 0:
        backendConfig.precision = static_cast<MNN::BackendConfig::PrecisionMode>(MNN::BackendConfig::PrecisionMode::Precision_Normal);
        break;
    case 1:
        backendConfig.precision = static_cast<MNN::BackendConfig::PrecisionMode>(MNN::BackendConfig::PrecisionMode::Precision_High);
        break;
    case 2:
        backendConfig.precision = static_cast<MNN::BackendConfig::PrecisionMode>(MNN::BackendConfig::PrecisionMode::Precision_Low);
        break;
    case 3:
        backendConfig.precision = static_cast<MNN::BackendConfig::PrecisionMode>(MNN::BackendConfig::PrecisionMode::Precision_Low_BF16);
        break;
    default:
        backendConfig.precision = static_cast<MNN::BackendConfig::PrecisionMode>(MNN::BackendConfig::PrecisionMode::Precision_Normal);
        break;
    }

    backendConfig.memory = static_cast<MNN::BackendConfig::MemoryMode>(MNN::BackendConfig::MemoryMode::Memory_Normal);
    backendConfig.power = static_cast<MNN::BackendConfig::PowerMode>(MNN::BackendConfig::PowerMode::Power_High);
    conf.backendConfig = &backendConfig;
    conf.backupType = MNNForwardType::MNN_FORWARD_CPU;
#ifdef __linux__
    if (backend == "x64")
    {
        conf.type = MNNForwardType::MNN_FORWARD_CPU;
        conf.numThread = num_threads;
        conf.backupType = MNNForwardType::MNN_FORWARD_CPU;
    }
#endif
#ifdef __ANDROID__
    if (backend == "arm")
    {
        conf.type = MNNForwardType::MNN_FORWARD_CPU;
        LOG(INFO) << "backend arm, num thread 4";
        conf.numThread = num_threads;
        conf.backupType = MNNForwardType::MNN_FORWARD_CPU;
    }
    else if (backend == "opencl")
    {
        conf.type = MNNForwardType::MNN_FORWARD_OPENCL;
        // 设置GPU Mode是image or buffer
        if (cl_mem_type == "cl_mem_buffer")
        {
            LOG(INFO) << "MNNGpuMode::MNN_GPU_TUNING_WIDE | MNNGpuMode::MNN_GPU_MEMORY_BUFFER";
            conf.mode = MNNGpuMode::MNN_GPU_TUNING_WIDE | MNNGpuMode::MNN_GPU_MEMORY_BUFFER;
        }
        else if (cl_mem_type == "cl_mem_image")
        {
            LOG(INFO) << "MNNGpuMode::MNN_GPU_TUNING_WIDE | MNNGpuMode::MNN_GPU_MEMORY_IMAGE";
            conf.mode = MNNGpuMode::MNN_GPU_TUNING_WIDE | MNNGpuMode::MNN_GPU_MEMORY_IMAGE;
        }
        else
        {
            LOG(INFO) << "MNNGpuMode::MNN_GPU_TUNING_WIDE | MNNGpuMode::MNN_GPU_MEMORY_BUFFER";
            conf.mode = MNNGpuMode::MNN_GPU_TUNING_WIDE | MNNGpuMode::MNN_GPU_MEMORY_BUFFER;
        }

        conf.backupType = MNNForwardType::MNN_FORWARD_CPU;
    }
    else if (backend == "nnapi")
    {
        conf.type = MNNForwardType::MNN_FORWARD_NN;
        conf.backupType = MNNForwardType::MNN_FORWARD_CPU;
    }
    else if (backend == "hw_npu")
    {
        // https://www.yuque.com/mnn/cn/xfs77m
        conf.type = MNNForwardType::MNN_FORWARD_USER_0;
        conf.backupType = MNNForwardType::MNN_FORWARD_CPU;
    }
#endif
    MNN::Session *session = interpreter->createSession(conf);
    benchmark(interpreter, session, num_warmup, num_runs);
}
