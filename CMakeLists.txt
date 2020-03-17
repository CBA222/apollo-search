cmake_minimum_required(VERSION 2.8.12)
project(MD5Encrypter)

add_definitions("-std=c++14")
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(src)
# add_executable(Service src/service.cpp src/search_index/index.cpp src/ranker/pageranker.cpp)
# target_link_libraries(Service ${CONAN_LIBS})

# add_executable(create_index src/create_index.cpp src/search_index/index.cpp)
# target_link_libraries(create_index ${CONAN_LIBS})

# add_executable(test_vector_space tests/vector_space_test.cpp)
# target_link_libraries(test_vector_space ${CONAN_LIBS})

# add_executable(vector_space_preprocess src/vector_space_preprocess.cpp)
# target_link_libraries(vector_space_preprocess ${CONAN_LIBS})

#[[
add_executable(
    service_test
    src/service_test.cpp
    src/data_service/mq/rr_broker.cpp
    src/logging/defer_logger.cpp
)
target_link_libraries(service_test ${CONAN_LIBS} pthread dl)

add_executable(
    service_test_2
    src/service_test_2.cpp
    src/logging/defer_logger.cpp
    src/data_service/mq/channel.cpp
    src/ranker/pageranker.cpp
    src/search_index/index.cpp
    src/data_service/ranker_service2.cpp
)
target_link_libraries(service_test_2 ${CONAN_LIBS} pthread dl)

]]

add_executable(
    service_test_3
    src/service_test_3.cpp
    src/logging/defer_logger.cpp
    src/data_service/mq/channel.cpp
    src/data_service/document_service.cpp
)
target_link_libraries(service_test_3 ${CONAN_LIBS} pthread dl)