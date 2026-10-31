cmake_minimum_required(VERSION 3.12)

project(LoadBalancer LANGUAGES CXX)

add_executable(LoadBalancer src/lb.cpp)
add_executable(Backend src/be.cpp)

foreach(target LoadBalancer Backend)
    target_compile_features(${target} PRIVATE cxx_std_20)
    set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE -Wall -Werror)
    endif()
endforeach()
