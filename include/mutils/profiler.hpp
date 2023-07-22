// #pragma once
#include "mutils/init.hpp"
#include <chrono>
#include <string>
#include <thread>
#include <functional>
#include <fstream>
#include <sstream>
#include <cfloat>
#include <cmath>

namespace huan
{

    namespace benchmark
    {

        struct RuntimeProileData
        {
            int64_t interval_time;
            int64_t current_value;
            int64_t voltage_value;
            int64_t memory_value;

            RuntimeProileData()
            {
                this->interval_time = 0;
                this->current_value = 0;
                this->voltage_value = 0;
                this->memory_value = 0;
            }
            RuntimeProileData(int64_t x, int64_t y, int64_t z, int64_t w)
            {
                this->interval_time = x;
                this->current_value = y;
                this->voltage_value = z;
                this->memory_value = w;
            }
        };

        class MyProfiler
        {
        private:
            std::chrono::high_resolution_clock::time_point start_time, end_time;

            /* control */
            bool is_monitor_running;
            std::thread power_monitor_thread;

        public:
            bool use_lat_profile = true;
            bool use_energy_profile = false;
            std::vector<huan::benchmark::RuntimeProileData> data;

            MyProfiler();
            /* data */
            void start();
            void end();

            void monitoring_power(const std::string current_file_path, const std::string voltage_file_path, const int interval, std::vector<RuntimeProileData> &power_data);

            double get_time();
            void debug();

            std::string unit = "us";
        };

        void compute_min_max_avg_std(const std::vector<double> arr, double &min_val, double &max_val, double &avg_val, double &std_val);
        std::string show_latency_metric(const std::vector<double> latency);
        void compute_energy_from_power(const std::vector<huan::benchmark::RuntimeProileData> power_data, double &energy);
        std::string show_energy_metric(const std::vector<double> power);
    }

}