#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

std::unordered_map<std::string, std::vector<int64_t>> parse_shape_info_int64(std::string input_str)
{
    std::istringstream iss(input_str);
    std::unordered_map<std::string, std::vector<int64_t>> dict;

    std::string token;
    while (std::getline(iss, token, ','))
    {
        std::istringstream iss2(token);
        std::string key;
        std::getline(iss2, key, ':');

        std::vector<int64_t> values;
        std::string value_str;
        while (std::getline(iss2, value_str, 'x'))
        {
            values.push_back(std::stoi(value_str));
        }
        dict[key] = values;
    }

    return dict;
}


std::unordered_map<std::string, std::vector<int>> parse_shape_info(std::string input_str)
{
    std::istringstream iss(input_str);
    std::unordered_map<std::string, std::vector<int>> dict;

    std::string token;
    while (std::getline(iss, token, ','))
    {
        std::istringstream iss2(token);
        std::string key;
        std::getline(iss2, key, ':');

        std::vector<int> values;
        std::string value_str;
        while (std::getline(iss2, value_str, 'x'))
        {
            values.push_back(std::stoi(value_str));
        }
        dict[key] = values;
    }

    return dict;
}