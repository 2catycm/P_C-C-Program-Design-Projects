//
// Created by 叶璨铭 on 2021/10/21.
//
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <util.h>
float result;

clock_t start;
#define CLOCK_START (start = clock())
#define CLOCK_END(msg) (printf(msg": %ld""ms, result: %f\n", (clock()-start), result))

//time_t start;
//#define CLOCK_START (start = time(NULL))
//#define CLOCK_END(msg) (printf(msg": %f""s, result: %f\n", difftime(time(NULL),start), result))

float dot_product(const float* x, const float* y, int size){
    float res = 0;
    for (int i = 0; i < size; ++i) {
        res += x[i] * y[i];
    }
    return res;
}
float mp(){
    float res = 0;
    int i;
//#pragma omp parallel for
//    for (i = 0; i < size; ++i) {
////#pragma omp critical
//        res += x[i] * y[i];
//    }
    i = 0;
    omp_set_num_threads(4);//默认是15个
    #pragma omp parallel
    {
//        printf("dot product %d", i++); //i被拷贝到15个线程，所以每个都是0
        printf("dot product %d",  omp_get_thread_num());
    }
    printf("omp_get_num_threads: %d", omp_get_num_threads());
    return res;
}
float dot_product3(const float* x, const float* y, int size){
    float res = 0, adder;
    int i;
    omp_set_num_threads(4);//默认是16个
#pragma omp parallel for
    for (i = 0; i < size; ++i) {
        adder = x[i] * y[i];
//#pragma omp critical //没有critical 变两秒
        res += adder;
    }
    return res;
}
float dot_product4(const float* x, const float* y, int size){
    float res = 0;
//    omp_set_num_threads(8);//默认是16个
//    int ts = (int)omp_get_num_threads(); //只能得到1，很垃圾。
    int *tsP = (int *)malloc(sizeof(int)); //只能得到1，很垃圾。
#pragma omp parallel
    {
        *tsP = omp_get_num_threads();
    };
    int ts = *tsP;
    float * const sums = (float*)calloc(ts,sizeof(float));
    memset(sums,0, sizeof(float)*ts);
#pragma omp parallel
    {
        int ct = omp_get_thread_num();
        for (int i = 0; i < size/ts; ++i) {
            sums[ct] += x[i+(size_t)ct*size/ts] * y[i+(size_t)ct*size/ts];
        }
    };
    for (int i = 0; i < ts; ++i) {
        res+=sums[i];
    }
    free(sums);
    return res;
}

void test1() {
//    float f1[] = {1,2};
//    float f2[] = {3,4};
//    int test_size = 1000000000;
    int test_size = 10000;
//    int test_size = 2000;
    float* f2 = (float*)calloc(test_size,sizeof(float));
//    memset(f2,2,sizeof(float)*test_size);
    float* f1 = (float*)calloc(test_size,sizeof(float));
//    memset(f1,1,sizeof(float)*test_size); //bad
//    for (int i = 0; i < test_size; ++i) {
//        printf("%f ", f1[i]);
//    }
    for (int i = 0; i < test_size; ++i) {
        f1[i] =1;
    }
    for (int i = 0; i < test_size; ++i) {
        f2[i] =1;
    }
    dot_product(f1,f2,test_size);
    dot_product(f1,f2,test_size);
    dot_product(f1,f2,test_size);
    CLOCK_START;
//    TimerStart;
    result = dot_product4(f1,f2,test_size);
//    TimerEndPrintlnStart("openmp");
    CLOCK_END("openmp");
//    CLOCK_START;
    result = dot_product(f1,f2,test_size);
//    TimerEndPrintln("trivial");
    CLOCK_END("trivial");
//    openmp: 7294ms, result: 16777216.000000
//    trivial: 3676ms, result: 16777216.000000 //win gcc
    //异常退出：msvc编译器
    //wsl gcc 58619016ms,

    //1000的情况下
//    openmp: 104356ms, result: 1617.000000  //time_t 有误
//    trivial: 27ms, result: 10000.000000

    free(f1);
    free(f2);
}
void test2() {
    mp();
}
int main(int argc, char **argv){
    test1();
    return 0;
}
//openmp: 1.000000s, result: 100000000.000000
//trivial: 0.000000s, result: 16777216.000000
