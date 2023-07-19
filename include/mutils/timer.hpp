#pragma once
#include <chrono>
#include <string>
#include <thread>
#include <functional>
#include <fstream>

#include "mutils/init.hpp"
#include "mutils/profile.hpp"
class MyTimer
{
private:
    std::chrono::high_resolution_clock::time_point start_time, end_time;

    /* control */
    // TODO 考虑加锁?
    bool is_monitor_running;
    std::thread power_monitor_thread;
    

public:

    std::vector<RuntimeProileData> data;

    MyTimer();
    /* data */
    void start();
    void end();

    void monitoring_power(const std::string current_file_path, const std::string voltage_file_path, const int interval, std::vector<RuntimeProileData> &power_data);

    double get_time();
    void debug();

    std::string unit = "us";
};

MyTimer::MyTimer()
{
    this->is_monitor_running = false;
}

void MyTimer::start()
{
    std::string current_file = FLAGS_android_current_file;
    std::string voltage_file = FLAGS_android_voltage_file;
    int32_t interval = FLAGS_monitor_interval;
    
    this->data.clear();
    this->is_monitor_running = true;
    this->power_monitor_thread = std::thread(&MyTimer::monitoring_power, this, current_file, voltage_file, interval, std::ref(this->data));

    this->start_time = std::chrono::high_resolution_clock::now();
}

void MyTimer::end()
{
    this->end_time = std::chrono::high_resolution_clock::now();

    this->is_monitor_running = false;
    this->power_monitor_thread.join();
}

double MyTimer::get_time()
{
    auto time_span = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    return time_span.count();
}

// NOTE,2023/07/19, only support for android (root)
void MyTimer::monitoring_power(const std::string current_file_path, const std::string voltage_file_path, const int32_t interval, std::vector<RuntimeProileData> &power_data)
{
    std::ifstream current_file, voltage_file;
    std::string current_value, voltage_value;
    std::chrono::high_resolution_clock::time_point tp1, tp2;
    tp1 = std::chrono::high_resolution_clock::now();
    while (this->is_monitor_running)
    {
        current_file.open(current_file_path);
        voltage_file.open(voltage_file_path);
        if (!current_file || !voltage_file)
        {
            LOG(ERROR) << "Read current or voltage failed!";
            continue;
        }

        std::getline(current_file, current_value);
        std::getline(voltage_file, voltage_value);
        tp2 = std::chrono::high_resolution_clock::now();
        int64_t time_spent = std::chrono::duration_cast<std::chrono::microseconds>(tp2 - tp1).count();
        tp1 = tp2;
        power_data.push_back(RuntimeProileData{time_spent, std::stoll(current_value), std::stoll(voltage_value), 0});
        current_file.close();
        voltage_file.close();

        std::this_thread::sleep_for(std::chrono::microseconds(interval));
    }
}

void  MyTimer::debug()
{
    LOG(INFO) << data.size();
    for (auto p_data : data)
    {
        LOG(INFO) << "time: " << p_data.interval_time << " "
                  << "current: " << p_data.current_value << " "
                  << "volatge: " << p_data.voltage_value;
    }
}