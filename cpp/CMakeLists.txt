cmake_minimum_required(VERSION 3.15)

project(MyProject)

add_executable(example example.cpp)

target_compile_features(example PRIVATE cxx_std_17)
target_include_directories(example PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(example PRIVATE ${CMAKE_SOURCE_DIR}/lib/libfourier_comm.a)
