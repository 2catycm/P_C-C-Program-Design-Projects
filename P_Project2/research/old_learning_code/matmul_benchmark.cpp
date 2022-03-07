//benchmark_demo.cpp
#include <iostream>
#include <cstring>
#include <benchmark/benchmark.h> //benchmark的头文件
//被测试的函数
static void test(benchmark::State& state){
    //数据准备
    auto i = state.range(0); //当前测试的数据规模
    //开始测试
    for (auto _: state) {//for内的代码会被计时。for会执行iterations指定的次数。
        int a[i];
        memset(a,0,sizeof(int)*i);
    }
//    for (auto _: state) {//for内的代码会被计时。for会执行iterations指定的次数。
//        long long a[i];
//        memset(a,0,sizeof(long long)*i);//分段可以吗
//    }
}
BENCHMARK(test)->Arg(32)->RangeMultiplier(2)->Range(32, 2048)->Iterations(10000); //Range2表示每次数据规模加倍；32和2048表示最小和最大的数据规模。
BENCHMARK_MAIN(); //代替main，输出测试结果