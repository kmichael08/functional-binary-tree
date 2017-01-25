cmake_minimum_required(VERSION 3.5)
project(functional_binary_tree)

set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -std=c++14")

set(EXAMPLE example.cc tree.h )
set(SOURCE_FILES tree_example.cc tree.h)
add_executable(functional_binary_tree ${SOURCE_FILES})
add_executable(example ${EXAMPLE})