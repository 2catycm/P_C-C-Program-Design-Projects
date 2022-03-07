//
// Created by 叶璨铭 on 2021/12/19.
//
#include "FaceClassifier/FaceClassifier.h"
#include "NeuronNetwork/NeuronNetwork.h"
#include <benchmark/benchmark.h>
#include <opencv2/opencv.hpp>
const std::string default_path = "../resources/samples/face.jpg";
struct BenchArg{
    static constexpr size_t WARMUP{0},
        MARK{1};
};
static void opencvPreProcess(benchmark::State& state) {
    size_t size = state.range(0);
    auto& fc = SUSTech::FaceClassifier::getInstance();
    for(auto _ : state) {
        const auto tensor = fc.tensorFromImage(cv::imread(default_path));
    }
}
BENCHMARK(opencvPreProcess)->Arg(BenchArg::WARMUP)->Arg(BenchArg::MARK)->Iterations(100);
//9ms

static void total(benchmark::State& state) {
    size_t size = state.range(0);
    auto& fc = SUSTech::FaceClassifier::getInstance();
    for(auto _ : state) {
        fc.classify(cv::imread(default_path));
    }
}
BENCHMARK(total)->Arg(BenchArg::WARMUP)->Arg(BenchArg::MARK)->Iterations(10*100);

BENCHMARK_MAIN();