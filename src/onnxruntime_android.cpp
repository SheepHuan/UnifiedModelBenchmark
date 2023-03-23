#include "core/session/onnxruntime_cxx_api.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cstdio>
#include "mutils/log.hpp"

char* copy_string(char *src){
    char * dst = (char *)malloc(sizeof(char)*strlen(src));
    memcpy(dst,src,sizeof(char)*strlen(src));
    return dst;
}

int run(Ort::Session &session, float *image, size_t image_size)
{
    std::vector<Ort::AllocatedStringPtr> ptrs;
    std::vector<Ort::Value> input_tensors;
    // std::vector<Ort::Value> output_tensors;
    std::vector<const char *> input_names;
    std::vector<const char *> output_names;
    Ort::AllocatorWithDefaultOptions allocator;
    size_t input_count = session.GetInputCount();
    size_t output_count = session.GetOutputCount();
    auto mem_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
    for (size_t i = 0; i < input_count; i++)
    {
        std::vector<int64_t> input_dim = session.GetInputTypeInfo(i).GetTensorTypeAndShapeInfo().GetShape();
        std::cout << "input dim: ";
        for (size_t j = 0; j < input_dim.size(); j++)
        {
            if (input_dim[j] == -1)
            {
                input_dim[j] = 1;
            }
            std::cout << input_dim[j] << " ";
        }
        std::cout << std::endl;
        Ort::AllocatedStringPtr input_name = session.GetInputNameAllocated(i, allocator);
        // ptrs.push_back(input_name);
        input_names.push_back(copy_string(input_name.get()));
        std::cout << input_names[i] << std::endl;
        input_tensors.push_back(
            Ort::Value::CreateTensor<float>(
                mem_info, image, image_size, input_dim.data(), input_dim.size()));
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

int main()
{
    std::string model_path = "/workspace/UnifiedModelBenchmark/samples/yolov4.onnx";
    std::string image_path = "/workspace/UnifiedModelBenchmark/samples/dog.jpg";
    Ort::Env env;
    Ort::SessionOptions session_options;
    session_options.EnableProfiling("result_");
    Ort::Session session_{env, model_path.c_str(), session_options}; // CPU
    cv::Mat image = cv::imread(image_path.c_str());
    // 图片从CV_8UC3 -> CV_32FC3
    image.convertTo(image, CV_32FC3, 1.0 / 255);
    cv::normalize(image, image, 0, 1, cv::NORM_MINMAX);
    // std::cout << image.type() << std::endl;
    run(session_, (float *)image.data, 3 * image.cols * image.rows);
    return 0;
}