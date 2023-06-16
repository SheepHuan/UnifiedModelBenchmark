#include "ncnn/net.h"
#include <gflags/gflags.h>
#include "mutils/log.hpp"
#include <vector>
DEFINE_string(graph, "", "ncnn model path");
DEFINE_string(param, "", "ncnn param path");
DEFINE_int32(warmup_runs, 3, "warmup_runs");
DEFINE_int32(num_runs, 10, "num_runs");
DEFINE_int32(num_threads, 2, "num_threads");
DEFINE_string(input_shape, "", "input_shape");

std::vector<int64_t> read_shape(std::string shape_str)
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


int main()
{
    // 解析命令行参数
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    std::string model_graph = FLAGS_graph;
    std::string model_prarm = FLAGS_param;
    int num_threads = FLAGS_num_threads;
    num_threads = std::min(num_threads, 8);
    int nums_warmup = FLAGS_nums_warmnup;
    int num_runs = FLAGS_nums_run;
    std::string input_shape_str = FLAGS_input_shape;
    mlog::info("graph", model_graph);
    mlog::info("param", model_prarm);
    mlog::info("num_threads:", std::to_string(num_threads));
    mlog::info("warmup_runs:", std::to_string(nums_warmup));
    mlog::info("num_runs:", std::to_string(num_runs));
    mlog::info("input_shape:", input_shape_str);

    std::vector<int64_t> input_shape = read_shape(input_shape_str);
    int64_t size = 1;
    for (auto dim:input_shape){
        size *=dim;
    }
    float_t* input_data = (float_t*)malloc(sizeof(float_t)*size);

    // // subtract 128, norm to -1 ~ 1
    // ncnn::Mat in = ncnn::Mat()
    // float mean[1] = { 128.f };
    // float norm[1] = { 1/128.f };
    // in.substract_mean_normalize(mean, norm);

    // ncnn::Net net;
    // net.load_param("model.param");
    // net.load_model("model.bin");

    // ncnn::Extractor ex = net.create_extractor();
    // ex.set_light_mode(true);
    // ex.set_num_threads(4);

    // ex.input("data", in);

    // ncnn::Mat feat;
    // ex.extract("output", feat);

    return 0;
}