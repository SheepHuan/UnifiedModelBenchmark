#include <vector>
#include <cmath>
#include <cfloat>
void profile_latency(std::vector<double> arr, int size, double &latency_min, double &latency_max, double &latency_avg, double &latency_std)
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
