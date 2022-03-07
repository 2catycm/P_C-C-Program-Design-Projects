//
// Created by 叶璨铭 on 2021/9/18.
//

#ifndef MUL_STOP_WATCH_HPP
#define MUL_STOP_WATCH_HPP

#include <iostream>
#include <chrono>

using namespace std::chrono; //仅作用于这个文件
class StopWatch{
private:
    time_point<high_resolution_clock> start_time;
public:
    inline
    StopWatch():start_time{high_resolution_clock::now()} {}
    void reset(){
        start_time = high_resolution_clock::now();
    }
    inline
    long long elapsedMicroSecond()
    {
        return duration_cast<microseconds>(high_resolution_clock::now() - start_time).count();
    }
};
#endif //MUL_STOP_WATCH_HPP
