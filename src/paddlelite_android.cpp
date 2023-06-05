#include "paddle_api.h"
#include <gflags/gflags.h>
#include "math.h"
#include "mutils/log.hpp"
using namespace paddle::lite_api;

DEFINE_string(graph, "", "paddlelite model path");
DEFINE_bool(graph_is_dir, false, "graph_is_dir");
DEFINE_int32(warmup_runs, 3, "warmup_runs");
DEFINE_int32(num_runs, 10, "num runs");
DEFINE_int32(num_threads, 4, "num threads");
DEFINE_int32(cpu_power_mode, 0, "cpu power mode");
DEFINE_bool(use_opencl, false, "use mobile opencl, otherwise use arm cpu");
// DEFINE_string(input_shape, "", "input_shape");
int64_t ShapeProduction(const shape_t &shape)
{
    int64_t res = 1;
    // printf("[ ");
    for (auto i : shape)
    {
        res *= i;
        // printf("%d ", i);
    }
    // printf("]\n");
    return res;
}

void run(std::shared_ptr<PaddlePredictor> predictor, int warmup_runs, int num_runs)
{
    std::vector<std::string> input_names = predictor->GetInputNames();
    int input_count = 0, output_count = 0;
    for (int i = 0; i < input_names.size(); i++)
    {
        printf("Input name[%d]: %s\n", i, input_names[i].c_str());
        input_count++;
        std::unique_ptr<Tensor> input_tensor(std::move(predictor->GetInput(0)));
        // ShapeProduction(input_tensor->shape());
        input_tensor->Resize({1, 3, 256, 256});
        auto *data = input_tensor->mutable_data<float>();
        for (int i = 0; i < ShapeProduction(input_tensor->shape()); ++i)
        {
            data[i] = 1;
        }
    }

    for (int i = 0; i < warmup_runs; i++)
    {
        predictor->Run();
    }
    std::vector<std::string> output_names = predictor->GetOutputNames();
    for (int i = 0; i < output_names.size(); i++)
    {
        printf("Output name[%d]: %s\n", i, output_names[i].c_str());
        std::unique_ptr<const Tensor> output_tensor(std::move(predictor->GetOutput(0)));
        ShapeProduction(output_tensor->shape());
        printf("[ ");
        for (auto i : output_tensor->shape())
        {
            printf("%d ", i);
        }
        printf("]\n");
        output_count++;
    }
}

int main(int argc, char **argv)
{
    // 解析命令行参数
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    std::string model_graph = FLAGS_graph;
    bool graph_is_dir = FLAGS_graph_is_dir;
    int cpu_power_mode = FLAGS_cpu_power_mode;
    int num_threads = FLAGS_num_threads;
    num_threads = std::min(num_threads, 8);
    bool use_opencl = FLAGS_use_opencl;
    int warmup_runs = FLAGS_warmup_runs;
    int num_runs = FLAGS_num_runs;
    mlog::info("model_file_path:", model_graph);
    mlog::info("num_threads:", std::to_string(num_threads));
    mlog::info("cpu_power_mode:", std::to_string(num_threads));
    mlog::info("backend:", use_opencl ? "opencl" : "arm");
    mlog::info("warmup_runs:", std::to_string(warmup_runs));
    mlog::info("num_runs:", std::to_string(num_runs));
    // 1. Set MobileConfig
    MobileConfig config;
    if (graph_is_dir)
    {
        config.set_model_dir(model_graph);
    }
    else
    {
        config.set_model_from_file(model_graph);
    }

    config.set_threads(num_threads);
    config.set_power_mode(static_cast<paddle::lite_api::PowerMode>(cpu_power_mode));

    std::shared_ptr<PaddlePredictor> predictor =
        CreatePaddlePredictor<MobileConfig>(config);
    run(predictor, warmup_runs, num_runs);
    return 0;
}