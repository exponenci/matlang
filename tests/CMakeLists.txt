cmake_minimum_required(VERSION 3.16)
project(m_test_fw)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(m_test_fw
        parser.cpp
        )
