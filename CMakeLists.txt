cmake_minimum_required(VERSION 3.23)
project(AcmeBag)

set(CMAKE_CXX_STANDARD 20)

add_executable(AcmeBag main.cpp ShoppingBag.cpp ShoppingBag.h Bag.cpp Bag.h)
