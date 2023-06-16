#include <vector>
#include <cmath>
void calc_std_deviation(std::vector<double> arr, int size,double& latency_avg ,double& latency_std) {
    double sum = 0.0, mean, stddev = 0.0;
    // double min_val,max_val;
    for(int i=0; i<size; ++i) {
        sum += arr[i];
    }

    mean = sum/size;

    for(int i=0; i<size; ++i) {
        stddev += pow(arr[i] - mean, 2);
    }
    latency_avg = mean;
    latency_std = sqrt(stddev/size);
    // return sqrt(stddev/size);
}

void calc_max_min(std::vector<double> arr, int size,double& latency_avg ,double& latency_std) {
    double sum = 0.0, mean, stddev = 0.0;
    // double min_val,max_val;
    for(int i=0; i<size; ++i) {
        sum += arr[i];
    }

    mean = sum/size;

    for(int i=0; i<size; ++i) {
        stddev += pow(arr[i] - mean, 2);
    }
    latency_avg = mean;
    latency_std = sqrt(stddev/size);
}