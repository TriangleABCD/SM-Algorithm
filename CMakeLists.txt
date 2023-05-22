cmake_minimum_required(VERSION 3.10)

project(sm_test CXX)

find_package(OpenMP REQUIRED)


include_directories(./include)

add_compile_options(-std=c++17 -Wall -O3)

aux_source_directory(./src SRC)

add_executable(sm_test ${SRC})

target_link_libraries(sm_test PRIVATE OpenMP::OpenMP_CXX)

set(CMAKE_BUILD_TYPE Debug)



