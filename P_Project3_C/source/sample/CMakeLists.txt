
add_executable(c_matmul main.c)
target_link_libraries(c_matmul matrix_multiplication)

add_executable(test_cmake test_cmake.c)
target_link_libraries(test_cmake util)

add_executable(testReadWrite testReadWrite.c)
target_link_libraries(testReadWrite matrix_multiplication)

add_executable(test_openmp test_openmp.c)

add_executable(test_strassen test_strassen.c)
target_link_libraries(test_strassen matrix_multiplication)

add_executable(matmul_benchmark matmul_benchmark.c)
target_link_libraries(matmul_benchmark matrix_multiplication)
