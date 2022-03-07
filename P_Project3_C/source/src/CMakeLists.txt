add_library(util util.c)

add_library(matmul_trivial matmul_trivial.c) #OpenMP 不需要链接，只需要编译选项
#find_package(OpenMP)
#if(OpenMP_C_FOUND)
#    target_link_libraries(matmul_trivial PUBLIC OpenMP::OpenMP_C)
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
#endif()

add_library(matmul_openblas matmul_openblas.c)
find_package(OpenBLAS CONFIG REQUIRED)
target_link_libraries(matmul_openblas PRIVATE OpenBLAS::OpenBLAS)
#target_link_libraries(matmul_openblas PUBLIC OpenBLAS::OpenBLAS)
#如果源文件(例如CPP)中包含第三方头文件，但是头文件（例如hpp）中不包含该第三方文件头，采用PRIVATE。
#如果源文件和头文件中都包含该第三方文件头，采用PUBLIC。

add_library(matmul_strassen matmul_strassen.c)
add_library(matmul_simd matmul_simd_openmp.c)

add_library(matrix_multiplication matrix_multiplication.c) #最后供main.c使用的是matrix_multiplication这个库
target_link_libraries(matrix_multiplication #这个库包括了矩阵乘法的不同实现方式
        util
        matmul_trivial
        matmul_openblas
        matmul_strassen
        matmul_simd)
