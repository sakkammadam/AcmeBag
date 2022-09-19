cmake_minimum_required(VERSION 3.23)
project(AcmeBag)

set(CMAKE_CXX_STANDARD 20)

add_executable(AcmeBag main.cpp Bag.cpp Bag.h)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")
set(PQXX /usr/include/pqxx)
find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)
target_link_libraries(AcmeBag ${PQXX_LIB} ${PQ_LIB})