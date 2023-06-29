#include <chrono>

class MyTimer
{
    std::chrono::high_resolution_clock::time_point start_time,end_time;
    public:
    MyTimer();
    /* data */
    void start();
    void end();
    double get_time();
};

MyTimer::MyTimer(){

}

void MyTimer::start(){
    start_time =  std::chrono::high_resolution_clock::now();
}

void MyTimer::end(){
    end_time =  std::chrono::high_resolution_clock::now();
}

double MyTimer::get_time(){
    auto time_span = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    return time_span.count();
}