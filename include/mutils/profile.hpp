#pragma once
#include <vector>
#include <cmath>
#include <cfloat>

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
    RuntimeProileData(int64_t x,int64_t y ,int64_t z ,int64_t w)
    {
        this->interval_time = x;
        this->current_value = y;
        this->voltage_value = z;
        this->memory_value = w;

    }
};

void
profile_latency(std::vector<double> arr, int size, double &latency_min, double &latency_max, double &latency_avg, double &latency_std)
{
    double sum = 0.0, mean, stddev = 0.0;
    latency_max = DBL_MIN, latency_min = DBL_MAX;
    for (int i = 0; i < size; ++i)
    {
        sum += arr[i];
        latency_max = arr[i] > latency_max ? arr[i] : latency_max;
        latency_min = arr[i] < latency_min ? arr[i] : latency_min;
    }

    mean = sum / size;

    for (int i = 0; i < size; ++i)
    {
        stddev += pow(arr[i] - mean, 2);
    }
    latency_avg = mean;
    latency_std = sqrt(stddev / size);
}


void compute_power(const std::vector<RuntimeProileData> data,double &total_energy){
    total_energy = 0;
    for (auto p_data : data)
    {
        // uA -> mA uV -> uV
        double current = 1e-3 *p_data.current_value;
        double voltage = 1e-3 *p_data.voltage_value;
        double power = current * voltage;
        // ms * mV * mA = mW*ms
        total_energy += 1e-6 * p_data.interval_time / 3600 * power;
    }
}

void
profile_energy(std::vector<double> arr, int size, double &latency_min, double &latency_max, double &latency_avg, double &latency_std)
{
    double sum = 0.0, mean, stddev = 0.0;
    latency_max = DBL_MIN, latency_min = DBL_MAX;
    for (int i = 0; i < size; ++i)
    {
        sum += arr[i];
        latency_max = arr[i] > latency_max ? arr[i] : latency_max;
        latency_min = arr[i] < latency_min ? arr[i] : latency_min;
    }

    mean = sum / size;

    for (int i = 0; i < size; ++i)
    {
        stddev += pow(arr[i] - mean, 2);
    }
    latency_avg = mean;
    latency_std = sqrt(stddev / size);
}
