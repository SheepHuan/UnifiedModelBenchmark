#include "mutils/init.hpp"
#include "mutils/timer.hpp"
#include "mutils/profile.hpp"
#include <thread>
int main(int argc, char **argv)
{
    init_env(argc, argv);

    MyTimer timer = MyTimer();
    /*
    ./test --android_current_file=/root/workspace/UnifiedModelBenchmark/tmp/1 --android_voltage_file=/root/workspace/UnifiedModelBenchmark/tmp/2
    */
    timer.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    timer.end();

    double energy;
    compute_power(timer.data,energy);
    LOG(INFO) << "total energy: "<< energy << " mWh";

    return 0;
}