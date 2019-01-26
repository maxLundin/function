cmake_minimum_required(VERSION 3.10)
project(function)

set(CMAKE_CXX_STANDARD 17)

add_executable(function main2.cpp
        function.h
	"gtest/gtest_main.cc"
	"gtest/gtest-all.cc"
	 "gtest/gtest.h"
        )

target_link_libraries(function -lpthread -Wall)
