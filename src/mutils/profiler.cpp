#include "mutils/profiler.hpp"

namespace huan
{

    namespace benchmark
    {
        MyProfiler::MyProfiler()
        {
            this->is_monitor_running = false;
        }

        void MyProfiler::start()
        {
            std::string current_file = FLAGS_android_current_file;
            std::string voltage_file = FLAGS_android_voltage_file;
            int32_t interval = FLAGS_monitor_interval;

#ifdef __ANDROID__
            if (this->use_energy_profile)
            {
                this->data.clear();
                this->is_monitor_running = true;
                this->power_monitor_thread = std::thread(&MyProfiler::monitoring_power, this, current_file, voltage_file, interval, std::ref(this->data));
            }
#endif

            this->start_time = std::chrono::high_resolution_clock::now();
        }

        void MyProfiler::end()
        {
            this->end_time = std::chrono::high_resolution_clock::now();
#ifdef __ANDROID__
            if (this->use_energy_profile)
            {
                this->is_monitor_running = false;
                this->power_monitor_thread.join();
            }
#endif
        }

        double MyProfiler::get_time()
        {
            auto time_span = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
            return time_span.count();
        }

        // NOTE,2023/07/19, only support for android (root)
        void MyProfiler::monitoring_power(const std::string current_file_path, const std::string voltage_file_path, const int32_t interval, std::vector<RuntimeProileData> &power_data)
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

        void MyProfiler::debug()
        {
            LOG(INFO) << data.size();
            for (auto p_data : data)
            {
                LOG(INFO) << "time: " << p_data.interval_time << " "
                          << "current: " << p_data.current_value << " "
                          << "volatge: " << p_data.voltage_value;
            }
        }

        void compute_min_max_avg_std(const std::vector<double> arr, double &min_val, double &max_val, double &avg_val, double &std_val)
        {
            double sum = 0.0, mean, stddev = 0.0;
            int size = arr.size();
            max_val = DBL_MIN, min_val = DBL_MAX;
            for (int i = 0; i < size; ++i)
            {
                sum += arr[i];
                max_val = arr[i] > max_val ? arr[i] : max_val;
                min_val = arr[i] < min_val ? arr[i] : min_val;
            }

            mean = sum / size;

            for (int i = 0; i < size; ++i)
            {
                stddev += pow(arr[i] - mean, 2);
            }
            avg_val = mean;
            std_val = sqrt(stddev / size);
        }

        std::string show_latency_metric(const std::vector<double> latency_data)
        {
            int rounds = latency_data.size();
            double min_val, max_val, avg_val, std_val;
            compute_min_max_avg_std(latency_data, min_val, max_val, avg_val, std_val);
            std::stringstream ss;
            ss << "rounds: " << rounds << ", min: " << min_val << ", max: " << max_val << ", avg: " << avg_val << " ,std: " << std_val << ", unit: us";
            return ss.str();
        }

        void compute_energy_from_power(const std::vector<huan::benchmark::RuntimeProileData> power_data, double &energy)
        {
            energy = 0;
            for (auto p_data : power_data)
            {
                // uA -> mA uV -> uV
                double current = 1e-3 * p_data.current_value;
                double voltage = 1e-3 * p_data.voltage_value;
                double power = current * voltage;
                // ms * mV * mA = mW*ms
                energy += 1e-6 * p_data.interval_time / 3600 * power;
            }
        }

        std::string show_energy_metric(const std::vector<double> power_data)
        {
            int rounds = power_data.size();
            double min_val, max_val, avg_val, std_val;
            compute_min_max_avg_std(power_data, min_val, max_val, avg_val, std_val);
            std::stringstream ss;
            ss << "rounds: " << rounds << ", min: " << min_val << ", max: " << max_val << ", avg: " << avg_val << " ,std: " << std_val << ", unit: mWh";
            return ss.str();
        }

    }
}