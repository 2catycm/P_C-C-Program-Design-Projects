cmake_minimum_required(VERSION 3.16)
project(P_Project4)
#C++语言标准
set(CMAKE_CXX_STANDARD 11)
if(MSVC)
#    message(status "msvc")
    #utf-8支持
    add_compile_definitions("UNICODE")
    add_compile_definitions("_UNICODE")
    add_compile_options("/utf-8")
    #OpenMP 和 SIMD 支持
    add_compile_options("/openmp:experimental")
    add_compile_options("/Qvec-report:2")
    if(CMAKE_BUILD_TYPE MATCHES Debug)
    else()
        add_compile_options("/O2")
    endif()
elseif(CMAKE_COMPILER_IS_GNUC)
    #OpenMP支持
    add_compile_options("-fopenmp")
    add_compile_options("-mavx2")
    add_compile_options("-fopt-info")
    if(CMAKE_BUILD_TYPE MATCHES Debug)
    else()
        add_compile_options("-Ofast")
    endif()
endif()

include_directories(
        ${PROJECT_SOURCE_DIR}/include/
)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_subdirectory(src)
add_subdirectory(sample)



