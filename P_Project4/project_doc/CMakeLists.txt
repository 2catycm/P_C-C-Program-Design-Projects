add_executable(mat_experiment 
old_practice_draft/opencv_tutorial_impl/0mat.cpp)
find_package(OpenCV CONFIG REQUIRED)
target_link_libraries(mat_experiment PRIVATE ${OpenCV_LIBS})