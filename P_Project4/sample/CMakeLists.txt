 find_package(OpenCV REQUIRED)
 message(STATUS "OpenCV library status:")
 message(STATUS "    config: ${OpenCV_DIR}")
 message(STATUS "    version: ${OpenCV_VERSION}")
 message(STATUS "    libraries: ${OpenCV_LIBS}")
 message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
 include_directories( ${OpenCV_INCLUDE_DIRS})

 add_executable(cv_example cv_example.cpp)
 target_link_libraries(cv_example PRIVATE ${OpenCV_LIBS})

 add_executable(cpp_language_test cpp_language_test.cpp)

add_executable(project_test project_test.cpp)
target_link_libraries(project_test SUSTechMat)

add_executable(construct construct.cpp)
target_link_libraries(construct SUSTechMat)

add_executable(R_Project4
        matmul_benchmark.cpp
)
find_package(benchmark CONFIG REQUIRED)
target_link_libraries(R_Project4 benchmark::benchmark benchmark::benchmark_main)
target_link_libraries(R_Project4 SUSTechMat)
