cmake_minimum_required(VERSION 3.12)

project(LoadBalancer LANGUAGES CXX)

add_executable(LoadBalancer
    src/lb.cpp
)

target_compile_features(LoadBalancer PRIVATE cxx_std_20)
set_target_properties(LoadBalancer PROPERTIES CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(LoadBalancer PRIVATE -Wall -Werror)
endif()
