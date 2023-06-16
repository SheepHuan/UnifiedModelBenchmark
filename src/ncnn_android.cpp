#include "ncnn/net.h"
#include <gflags/gflags.h>
#include "easylogging++.h"
#include <vector>
DEFINE_string(graph, "", "ncnn model path");
DEFINE_string(param, "", "ncnn param path");
DEFINE_string(backend, "arm", "arm,opencl,vulkan");
DEFINE_int32(num_threads, 2, "num_threads");
DEFINE_int32(nums_warmup, 3, "warmup_runs");
DEFINE_int32(num_runs, 10, "num_runs");
DEFINE_string(input_shape, "", "input_shape");

INITIALIZE_EASYLOGGINGPP

void print_args()
{
    std::string model_path = FLAGS_graph;
    std::string param_path = FLAGS_param;
    std::string backend = FLAGS_backend;
    int num_threads = FLAGS_num_threads;
    int nums_warmup = FLAGS_nums_warmup;
    int num_runs = FLAGS_num_runs;
    std::string input_shape = FLAGS_input_shape;
    LOG(INFO) << "model path: " << model_path;
    LOG(INFO) << "model path: " << param_path;
    LOG(INFO) << "backend: " << backend;
    if (backend == "arm")
    {
        LOG(INFO) << "cpu threads: " << num_threads;
    }

    LOG(INFO) << "nums_warmup: " << nums_warmup;
    LOG(INFO) << "num_runs: " << num_runs;
    LOG(INFO) << "input_shape: " << input_shape;
}

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

void run(const char *model_path, const char *param_path)
{
    ncnn::Net net;
    if (net.load_model(model_path) == -1)
    {
        LOG(ERROR) << "fail load graph: " << model_path;
    }
    if (net.load_param(param_path) == -1)
    {
        LOG(ERROR) << "fail load param: " << model_path;
    }
    const std::vector<const char *> &input_names = net.input_names();
    const std::vector<const char *> &output_names = net.output_names();

    for (int i = 0; i < input_names.size(); i++)
    {
        LOG(INFO) << i << " input: " << input_names[i];
    }

    for (int i = 0; i < output_names.size(); i++)
    {
        LOG(INFO) << i << " output: " << output_names[i];
    }
}

int main(int argc, char **argv)
{
    // 解析命令行参数
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    std::string model_path = FLAGS_graph;
    std::string param_path = FLAGS_param;
    std::string backend = FLAGS_backend;
    int num_threads = FLAGS_num_threads;
    int nums_warmup = FLAGS_nums_warmup;
    int num_runs = FLAGS_num_runs;
    // std::string input_shape_str = FLAGS_input_shape;

    print_args();
    if (backend == "arm")
    {
        run(model_path.c_str(), param_path.c_str());
    }
    else if (backend == "opencl")
    {
        LOG(WARNING) << "opencl backend is not avaliable now!";
    }
    else if (backend == "vulkan")
    {
        LOG(WARNING) << "vulkan backend is not avaliable now!";
    }
    else
    {
        LOG(ERROR) << "unkown backend for ncnn: " << backend;
    }

    // std::vector<int64_t> input_shape = read_shape(input_shape_str);
    // int64_t size = 1;
    // for (auto dim:input_shape){
    //     size *=dim;
    // }
    // float_t* input_data = (float_t*)malloc(sizeof(float_t)*size);

    // TODO
    //  ncnn::Mat in;
    //  ncnn::Mat out;

    // warm up
    // for (int i = 0; i < nums_warmup; i++)
    // {
    //     ncnn::Extractor ex = net.create_extractor();
    //     ex.input(input_names[0], in);
    //     ex.extract(output_names[0], out);
    // }

    //  for (int i = 0; i < num_runs; i++)
    // {
    //     {
    //         ncnn::Extractor ex = net.create_extractor();
    //         ex.input(input_names[0], in);
    //         ex.extract(output_names[0], out);
    //     }
    // }

    return 0;
}