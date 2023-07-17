#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <regex>
#include <typeinfo>
#include "args.hpp"
// #include "log.hpp"
#include <stdexcept>
#include <assert.h>


namespace huan
{
    namespace benchmark
    {


        int64_t shape_production(const std::vector<int64_t> &shape)
        {
            int64_t res = 1;
            for (auto i : shape)
            {
                res *= i;
            }
            return res;
        }

        int64_t shape_production(const std::vector<int> &shape)
        {
            int64_t res = 1;
            for (auto i : shape)
            {
                res *= i;
            }
            return res;
        }

        std::vector<int> convert_shape_pd2ncnn(std::vector<int64_t> shape)
        {
            std::vector<int> ncnn_shape;
            for (int i = 0; i < shape.size(); i++)
            {
                ncnn_shape.push_back((int)shape[i]);
            }
            return ncnn_shape;
        }

        struct MTensorInfo
        {
            std::string name;
            std::vector<int64_t> shape;
            std::string dtype;

            MTensorInfo()
            {
                name = "";
                shape = {};
                dtype = "";
            }
        };

        struct MTensorDict
        {
            std::vector<huan::benchmark::MTensorInfo> inputs_info;
            MTensorDict(std::vector<huan::benchmark::MTensorInfo> _info):inputs_info(_info)
            {
        
            }
            std::vector<int64_t> query_pd_shape_info(std::string tensor_name)
            {
                for (int i = 0; i < inputs_info.size(); i++)
                {
                    // if (IS_DEBUG){
                    //     LOG(INFO)<<__FILE__<<"#"<< __LINE__<<":"<<inputs_info[i].name<<","<<tensor_name;
                    // }
                    if (inputs_info[i].name == tensor_name)
                    {
                        return inputs_info[i].shape;
                    }
                }
                return {};
            }
            std::vector<int> query_ncnn_shape_info(std::string tensor_name)
            {
                for (int i = 0; i < inputs_info.size(); i++)
                {
                    if (inputs_info[i].name == tensor_name)
                    {
                        return convert_shape_pd2ncnn(inputs_info[i].shape);
                    }
                }
                return {};
            }
        };

        int64_t parse_shape_info(std::string input_str, std::vector<int64_t> &output_shape)
        {

            try
            {
                std::stringstream ss(input_str);
                std::string token;

                while (std::getline(ss, token, 'x'))
                {
                    int number = std::stoi(token);
                    output_shape.push_back((int64_t)number);
                }
            }
            catch (const std::exception &e)
            {
                LOG(ERROR) << e.what();
                return -1;
            }
            return 0;
        }

        int64_t parse_tensor_info(std::string input_str, std::vector<MTensorInfo> &output_tensor_info)
        {
            int64_t ret_code = 0;
            try
            {
                std::regex pattern("\\[(.*?),(.*?),(.*?)\\]");
                std::sregex_iterator iter(input_str.begin(), input_str.end(), pattern);
                std::sregex_iterator end;

                // 遍历匹配结果
                while (iter != end)
                {
                    
                    MTensorInfo tensor_info;
                    // 获取匹配结果的各个部分
                    std::smatch match = *iter;
                    std::string str_name = match[1].str();
                    std::string str_shape = match[2].str();
                    std::string str_type = match[3].str();
        
                    // 输出结果
                    std::vector<int64_t> shape;
                    ret_code = parse_shape_info(str_shape, shape);
                    if (ret_code != 0)
                    {
                        throw std::runtime_error("parse_shape_info error");
                    }
                    tensor_info.name = str_name;
                    tensor_info.dtype = str_type;
                    tensor_info.shape = shape;
                    output_tensor_info.push_back(tensor_info);
                    // 移动到下一个匹配结果
                    ++iter;
                }
            }
            catch (const std::exception &e)
            {
                LOG(ERROR) << e.what();
                return -1;
            }
            return 0;
        }
    }

}