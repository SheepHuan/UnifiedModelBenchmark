#include "core/session/onnxruntime_cxx_api.h"
#include "opencv2/opencv.hpp"
#include <iostream>



int main()
{
    std::string model_path = "/workspace/UnifiedModelBenchmark/samples/yolov4.onnx";

    Ort::Env env;
    Ort::SessionOptions session_options;
    session_options.EnableProfiling("result_");
    Ort::Session session_{env, model_path.c_str(), session_options}; // CPU

    size_t input_count = session_.GetInputCount();
    size_t output_count = session_.GetOutputCount();
    std::cout << input_count << std::endl;
    return 0;
}