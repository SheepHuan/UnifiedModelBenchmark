#include "ncnn/net.h"
#include "ncnn/blob.h"
#include "cpu.h"
#include "gpu.h"
#include <iostream>
#include "mutils/tensor.hpp"
#include "mutils/profile.hpp"
#include "mutils/timer.hpp"
#include "mutils/args.hpp"
#include "mutils/log.hpp"


void print_args()
{
    std::string model_path = FLAGS_model;
    std::string param_path = FLAGS_param;
    std::string backend = FLAGS_backend;
    int num_threads = FLAGS_num_threads;
    int nums_warmup = FLAGS_num_warmup;
    int num_runs = FLAGS_num_runs;
    std::string input_info = FLAGS_input_info;
    // std::string output_info = FLAGS_output_info;
    LOG(INFO) << "=================================\t"
              << "Args Info"
              << "\t=================================";
    LOG(INFO) << "model path: " << model_path;
    LOG(INFO) << "param path: " << param_path;
    LOG(INFO) << "backend: " << backend;
    LOG(INFO) << "cpu threads: " << num_threads;
    LOG(INFO) << "nums_warmup: " << nums_warmup;
    LOG(INFO) << "num_runs: " << num_runs;
    LOG(INFO) << "input_info: " << input_info;
    // LOG(INFO) << "output_info: " << output_info;
}

static ncnn::VulkanDevice *g_vkdev = 0;
static ncnn::VkAllocator *g_blob_vkallocator = 0;
static ncnn::VkAllocator *g_staging_vkallocator = 0;
void run(const char *model_path, const char *param_path,
         huan::benchmark::MTensorDict input_tensors_info,
         int nums_warmup, int num_runs,
         ncnn::Option opt)
{
    LOG(INFO) << "=================================\t"
              << "Runtime Info"
              << "\t=================================";
    ncnn::Net net;
    net.opt = opt;

    if (net.opt.use_vulkan_compute)
    {

        g_blob_vkallocator->clear();
        g_staging_vkallocator->clear();
        net.set_vulkan_device(g_vkdev);
    }

    // NOTE 这里必须先加载param，再加载bin
    if (net.load_param(param_path) == -1)
    {
        LOG(ERROR) << "fail load param: " << model_path;
    }
    if (net.load_model(model_path) == -1)
    {
        LOG(ERROR) << "fail load model: " << model_path;
    }

    const std::vector<const char *> input_names = net.input_names();
    const std::vector<const char *> output_names = net.output_names();
    const std::vector<ncnn::Mat> input_mats;

    for (int i = 0; i < input_names.size(); i++)
    {
        // 强制要求batch=1,n,c,h,w
        std::vector<int> shape = input_tensors_info.query_ncnn_shape_info(input_names[i]);
        LOG(INFO) << i << " input: " << input_names[i] << ", WxHxDxC: " << shape[3] << "x" << shape[2] << "x" << 1 << "x" << shape[1];
    }

    MyTimer timer = MyTimer();
    double warmup_time = 0;
    double latency_avg = 0, latency_std = 0, latency_max = DBL_MIN, latency_min = DBL_MAX;
    std::vector<double> latency_per_rounds;
    for (int i = 0; i < nums_warmup; i++)
    {
        ncnn::Extractor ex = net.create_extractor();
        // 输入多个输入
        timer.start();
        for (int i = 0; i < input_names.size(); i++)
        {
            std::vector<int> shape = input_tensors_info.query_ncnn_shape_info(input_names[i]);
            ncnn::Mat in = ncnn::Mat(shape[3], shape[2], 1, shape[1]);
            in.fill(0.5f);
            ex.input(input_names[i], in);
        }
        for (int i = 0; i < output_names.size(); i++)
        {
            ncnn::Mat out;
            ex.extract(output_names[i], out);
        }
        timer.end();
        warmup_time = warmup_time + timer.get_time();
    }

    for (int i = 0; i < num_runs; i++)
    {
        timer.start();
        {
            ncnn::Extractor ex = net.create_extractor();

            for (int i = 0; i < input_names.size(); i++)
            {
                std::vector<int> shape = input_tensors_info.query_ncnn_shape_info(input_names[i]);
                ncnn::Mat in = ncnn::Mat(shape[3], shape[2], 1, shape[1]);
                in.fill(0.5f);
                ex.input(input_names[i], in);
            }
            for (int i = 0; i < output_names.size(); i++)
            {
                ncnn::Mat out;
                ex.extract(output_names[i], out);
            }
        }
        timer.end();
        latency_per_rounds.push_back(timer.get_time());
    }
  
    profile_latency(latency_per_rounds, latency_per_rounds.size(),latency_min,latency_max, latency_avg, latency_std);
    LOG(INFO) << "warmup: " << nums_warmup << " rounds, avg time: " << warmup_time * 1.0 / nums_warmup << " us";
    LOG(INFO) << "run: " << num_runs << " rounds, min: " << latency_min << " us, max: " << latency_max << " us, avg : " << latency_avg << " us, std: " << latency_std << " us";
}

int main(int argc, char **argv)
{
    // 解析命令行参数
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    std::string model_path = FLAGS_model;
    std::string param_path = FLAGS_param;
    std::string backend = FLAGS_backend;
    int num_threads = FLAGS_num_threads;
    int nums_warmup = FLAGS_num_warmup;
    int num_runs = FLAGS_num_runs;
    std::string str_input_info = FLAGS_input_info;
  
    std::vector<huan::benchmark::MTensorInfo> input_tensors_info;
    huan::benchmark::parse_tensor_info(str_input_info, input_tensors_info);
    huan::benchmark::MTensorDict input_tensors_dict(input_tensors_info);

    print_args();

    // 设置CPU状态
    // https://github.com/Tencent/ncnn/wiki/openmp-best-practice
    ncnn::set_cpu_powersave(2);
    ncnn::set_omp_dynamic(0);
    ncnn::set_omp_num_threads(num_threads);

    ncnn::Option opt;
    opt.lightmode = true;
    opt.num_threads = num_threads;

    // opt.blob_allocator = &g_blob_pool_allocator;
    // opt.workspace_allocator = &g_workspace_pool_allocator;

    opt.use_winograd_convolution = true;
    opt.use_sgemm_convolution = true;
    opt.use_int8_inference = true;

    opt.use_fp16_packed = true;
    opt.use_fp16_storage = true;
    opt.use_fp16_arithmetic = true;
    opt.use_int8_storage = true;
    opt.use_int8_arithmetic = true;
    opt.use_packing_layout = true;
    opt.use_shader_pack8 = false;
    opt.use_image_storage = false;

    if (backend == "arm")
    {
        opt.use_vulkan_compute = false;
    }
    else if (backend == "vulkan")
    {
        opt.use_vulkan_compute = true;
        g_vkdev = ncnn::get_gpu_device(0);
        g_blob_vkallocator = new ncnn::VkBlobAllocator(g_vkdev);
        g_staging_vkallocator = new ncnn::VkStagingAllocator(g_vkdev);
        opt.blob_vkallocator = g_blob_vkallocator;
        opt.workspace_vkallocator = g_blob_vkallocator;
        opt.staging_vkallocator = g_staging_vkallocator;
        // run(model_path.c_str(), param_path.c_str(), input_tensors_info, nums_warmup, num_runs, opt);
    }
    else
    {
        LOG(ERROR) << "unkown backend for ncnn: " << backend;
    }
    run(model_path.c_str(), param_path.c_str(), input_tensors_info, nums_warmup, num_runs, opt);
    return 0;
}