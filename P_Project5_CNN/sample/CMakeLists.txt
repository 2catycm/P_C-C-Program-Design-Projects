 find_package(OpenCV REQUIRED)
 message(STATUS "OpenCV library status:")
 message(STATUS "    config: ${OpenCV_DIR}")
 message(STATUS "    version: ${OpenCV_VERSION}")
 message(STATUS "    libraries: ${OpenCV_LIBS}")
 message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
 include_directories(${OpenCV_INCLUDE_DIRS})
 add_executable(opencv_conv experiment/opencv_conv.cpp)
 target_link_libraries(opencv_conv PRIVATE ${OpenCV_LIBS})



 add_executable(matrix_test matrix_test.cpp)
 target_link_libraries(matrix_test PRIVATE ${OpenCV_LIBS})

 add_executable(fc_main fc_main.cpp)
# target_link_libraries(fc_main FaceClassifier)
  target_link_libraries(fc_main PRIVATE ${OpenCV_LIBS})

 add_executable(fc_video_stream fc_video_stream.cpp)
 target_link_libraries(fc_video_stream PRIVATE ${OpenCV_LIBS})

 find_package(benchmark CONFIG REQUIRED)
 add_executable(fc_nn_benchmark fc_nn_benchmark.cpp)
 target_link_libraries(fc_nn_benchmark PRIVATE ${OpenCV_LIBS})
 target_link_libraries(fc_nn_benchmark PRIVATE benchmark::benchmark benchmark::benchmark_main)


 add_executable(test_move test_move.cpp)
 target_link_libraries(test_move PRIVATE ${OpenCV_LIBS})

# add_executable(NetWorkTest NetWorkTest.cpp)
# target_link_libraries(test_move PRIVATE ${OpenCV_LIBS})

 add_executable(cnn_compute IntroductionAIHomework/cnn_compute.cpp)