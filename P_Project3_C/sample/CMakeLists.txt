
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
include_directories( ${OpenCV_INCLUDE_DIRS})

add_executable(c_matmul main.c)
target_link_libraries(c_matmul matrix_multiplication)

add_executable(test_cmake test_cmake.c)
target_link_libraries(test_cmake util)

add_executable(testReadWrite testReadWrite.c)
target_link_libraries(testReadWrite matrix_multiplication)

add_executable(test_openmp test_openmp.c)

add_executable(test_strassen test_strassen.c)
target_link_libraries(test_strassen matrix_multiplication)



add_executable(matmul_benchmark matmul_benchmark.cpp)
target_link_libraries(matmul_benchmark matrix_multiplication )
target_link_libraries(matmul_benchmark ${OpenCV_LIBS} )


add_executable(matadd_benchmark_lab8 matadd_benchmark_lab8.c)
target_link_libraries(matadd_benchmark_lab8 matrix_multiplication)
