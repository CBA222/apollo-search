cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Apollo-Search VERSION 1.0)

include_directories(src)
add_executable(Service src/service.cpp src/search_index/index.cpp src/ranker/pageranker.cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Service Threads::Threads)



