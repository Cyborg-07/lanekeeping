#cmake_minimum_required(VERSION 3.17)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

add_executable(untitled main.cpp)

cmake_minimum_required(VERSION 3.5)
#project(avlexample)

find_package(
        AVL
        # for a specific version, uncomment the line below
        #4.11
        CONFIG
        REQUIRED
)

#add_executable(
#        # executable name
#        example_exec
#        # source files
#        main.cpp
#)

target_link_libraries(
        untitled
        PUBLIC
        AVL
)