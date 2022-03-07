cmake_minimum_required(VERSION 3.20)
project(P_Project2)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>") #告诉msvc，我要utf-8

set(CMAKE_CXX_STANDARD 14)

add_executable(P_Project2
        main.cpp
        )
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(P_Project2 PUBLIC OpenMP::OpenMP_CXX)
endif()
#
#add_executable(R_Project2
#        matmul_benchmark.cpp
#)
#find_package(benchmark REQUIRED)
#target_link_libraries(R_Project2 PRIVATE benchmark::benchmark)
##add_executable(testEncoding
##        ./编码问题研究/testEncoding.cpp
##)
#add_executable(testMacro
#        cross_platform/testMacro.cpp
#)
#add_executable(openmp_demo
#        openmp_demo/openmp.cpp)
#find_package(OpenMP)
#if(OpenMP_CXX_FOUND)
#    target_link_libraries(openmp_demo PUBLIC OpenMP::OpenMP_CXX)
#endif()
