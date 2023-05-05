#include "core/session/onnxruntime_cxx_api.h"
#include "core/session/onnxruntime_c_api.h"
#ifdef ANDROID_PLATFORM
#include "providers/nnapi/nnapi_provider_factory.h"
#endif
#include "opencv2/opencv.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>
#include "mutils/log.hpp"
#include <gflags/gflags.h>

DEFINE_string(model_path, "", "*.onnx");
DEFINE_string(image_path, "", "*.png/*.jpg");
// DEFINE_string(image_shape, "", "1x3x112x112");
// DEFINE_string(data_type, "", "float32");
DEFINE_string(prefix, "", "result");
DEFINE_bool(use_nnapi, false, "use nnapi");
char *copy_string(char *src)
{
    char *dst = (char *)malloc(sizeof(char) * strlen(src));
    memcpy(dst, src, sizeof(char) * strlen(src));
    return dst;
}

int run(Ort::Session &session)
{
    std::vector<Ort::AllocatedStringPtr> ptrs;
    std::vector<Ort::Value> input_tensors;
    std::vector<const char *> input_names;
    std::vector<const char *> output_names;
    Ort::AllocatorWithDefaultOptions allocator;
    size_t input_count = session.GetInputCount();
    size_t output_count = session.GetOutputCount();
    auto mem_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
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
        Ort::AllocatedStringPtr input_name = session.GetInputNameAllocated(i, allocator);
        input_names.push_back(copy_string(input_name.get()));

        std::cout << input_names[i] << std::endl;
        // 支持
        switch (data_type)
        {
        case 1:
            float *float32_data = (float *)malloc(sizeof(float) * size);
            input_tensors.push_back(
                Ort::Value::CreateTensor<float>(
                    mem_info, float32_data, size, input_dim.data(), input_dim.size()));
            break;
        case 3:
            int8_t *int8_data = (int8_t *)malloc(sizeof(int8_t) * size);
            input_tensors.push_back(
                Ort::Value::CreateTensor<int8_t>(
                    mem_info, int8_data, size, input_dim.data(), input_dim.size()));
            break;
        case 2:
            uint8_t *uint8_data = (uint8_t *)malloc(sizeof(uint8_t) * size);
            input_tensors.push_back(
                Ort::Value::CreateTensor<uint8_t>(
                    mem_info, uint8_data, size, input_dim.data(), input_dim.size()));
            break;

        default:
            break;
        }
    }

    for (size_t i = 0; i < output_count; i++)
    {
        Ort::AllocatedStringPtr output_name = session.GetOutputNameAllocated(i, allocator);
        output_names.push_back(copy_string(output_name.get()));
        std::cout << output_names[i] << std::endl;
    }
    auto output_tensors = session.Run(Ort::RunOptions{nullptr}, input_names.data(), input_tensors.data(), input_count, output_names.data(), output_count);
    return 0;
}

int main(int argc, char **argv)
{
    // 解析命令行参数
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    std::string model_path = FLAGS_model_path;
    std::string image_path = FLAGS_image_path;
    std::string result_prefix = FLAGS_prefix;
    // std::string shape = FLAGS_image_shape;
    // std::stringstream ss(shape);
    // int n, c, h, w;
    // char sep;
    // ss >> n >> sep >> c >> sep >> h >> sep >> w;

    // if (!ss.fail() && ss.eof())
    // {
    //     std::cout << "n = " << n << ", c = " << c << ", h = " << h << ", w = " << w << std::endl;
    // }
    // else
    // {
    //     std::cerr << "Invalid shape format: " << shape << std::endl;
    //     return 1;
    // }

    Ort::Env env = Ort::Env{ORT_LOGGING_LEVEL_ERROR, "Default"};
    Ort::SessionOptions session_options;
    session_options.EnableProfiling(result_prefix.c_str());
    // 注册NNApi
    if (FLAGS_use_nnapi)
    {

/*
这里可以设置文档地址(https://onnxruntime.ai/docs/execution-providers/NNAPI-ExecutionProvider.html)，例如下:
nnapi_flags |= NNAPI_FLAG_USE_FP16;
*/
#ifdef ANDROID_PLATFORM
        uint32_t nnapi_flags = 0;
        nnapi_flags |= NNAPI_FLAG_CPU_ONLY;
        Ort::ThrowOnError(OrtSessionOptionsAppendExecutionProvider_Nnapi(session_options, nnapi_flags));
#else
        mlog::error("Current Complier is not for android!");
#endif
    }

    Ort::Session session{env, model_path.c_str(), session_options}; // CPU

    // cv::Mat image = cv::imread(image_path.c_str());
    // // 图片从CV_8UC3 -> CV_32FC3
    // image.convertTo(image, CV_32FC3, 1.0 / 255);
    // cv::resize(image, image, cv::Size(w, h));
    // cv::normalize(image, image, 0, 1, cv::NORM_MINMAX);
    // std::cout << image.type() << std::endl;
    run(session);
    return 0;
}