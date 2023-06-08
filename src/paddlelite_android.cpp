#include "paddle_api.h"
// #include "paddle_use_passes.h" // NOLINT
#include <gflags/gflags.h>
#include "math.h"
#include "mutils/log.hpp"
#include "mutils/timer.hpp"
#include <sstream>
#include <iostream>
using namespace paddle::lite_api;

DEFINE_string(graph, "", "paddlelite model path");
DEFINE_bool(graph_is_dir, false, "graph_is_dir");
DEFINE_string(backend, "arm", "use mobile opencl, otherwise use arm cpu");
DEFINE_int32(nums_warmnup, 3, "warmup_runs");
DEFINE_int32(nums_run, 10, "num runs");
DEFINE_int32(num_threads, 4, "num threads");
DEFINE_int32(cpu_power_mode, 0, "cpu power mode");
DEFINE_string(input_shape, "", "input_shape");
int64_t ShapeProduction(const shape_t &shape)
{
    int64_t res = 1;
    for (auto i : shape)
    {
        res *= i;
    }
    return res;
}

shape_t read_shape(std::string shape_str)
{
    std::vector<int64_t> shape;
    std::stringstream ss(shape_str);
    std::string token;
    while (getline(ss, token, ','))
    {
        int i = std::stoi(token);

        shape.push_back(i);
    }

    return shape;
}

void run(std::shared_ptr<PaddlePredictor> predictor, int warmup_runs, int num_runs)
{
    std::string input_shape_str = FLAGS_input_shape;
    std::cout << input_shape_str << std::endl;
    shape_t xshape = read_shape(input_shape_str);

    std::vector<std::string> input_names = predictor->GetInputNames();
    int input_count = 0, output_count = 0;
    for (int i = 0; i < input_names.size(); i++)
    {
        printf("Input name[%d]: %s\n", i, input_names[i].c_str());
        input_count++;
        std::unique_ptr<Tensor> input_tensor(std::move(predictor->GetInput(0)));

        input_tensor->Resize(xshape);
        auto *data = input_tensor->mutable_data<float>();
        for (int i = 0; i < ShapeProduction(input_tensor->shape()); ++i)
        {
            data[i] = 1;
        }
    }
    MyTimer timer = MyTimer();
    for (int i = 0; i < num_runs; i++)
    {
        timer.start();
        predictor->Run();
        timer.end();
        printf("round: %d, lat: %0.3f\n", i, timer.get_time());
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
    std::cout<<"hello"<<std::endl;
    // 解析命令行参数
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    std::string model_graph = FLAGS_graph;
    bool graph_is_dir = FLAGS_graph_is_dir;
    int cpu_power_mode = FLAGS_cpu_power_mode;
    int num_threads = FLAGS_num_threads;
    num_threads = std::min(num_threads, 8);
    std::string backend = FLAGS_backend;
    int nums_warmup = FLAGS_nums_warmnup;
    int num_runs = FLAGS_nums_run;
    mlog::info("graph", model_graph);
    mlog::info("num_threads:", std::to_string(num_threads));
    mlog::info("cpu_power_mode:", std::to_string(num_threads));
    mlog::info("backend:", backend);
    mlog::info("warmup_runs:", std::to_string(nums_warmup));
    mlog::info("num_runs:", std::to_string(num_runs));

    CxxConfig config;
    if (graph_is_dir)
    {
        config.set_model_dir(model_graph);
    }
    else
    {
        // config.set_model_file(model_graph);
        // config.set_model_from_file(model_graph);
    }
    std::vector<Place> valid_places;
    if (backend == "arm")
    {
        valid_places.emplace_back(Place{TARGET(kARM), PRECISION(kFloat)});
        config.set_threads(num_threads);
        config.set_power_mode(static_cast<paddle::lite_api::PowerMode>(cpu_power_mode));
    }
    else if (backend == "opencl")
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
    else
    {
        return -1;
    }

    config.set_valid_places(valid_places);

    std::shared_ptr<PaddlePredictor> predictor =
        CreatePaddlePredictor<CxxConfig>(config);
    run(predictor, nums_warmup, num_runs);
    return 0;
}