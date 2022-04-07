#cmake_minimum_required(VERSION 3.16)
#project(matlang)
#
#set(CMAKE_CXX_STANDARD 20)
#
#include_directories(. include src types types/include types/src)
#
#add_library(MatLang STATIC "")
#
#add_subdirectory(types)
#add_subdirectory(include)
#add_subdirectory(src)
#
##link_libraries(MatLang)
#add_executable(${PROJECT_NAME} main.cpp)
#target_link_libraries(${PROJECT_NAME} PRIVATE MatLang)
#
##target_sources(matlang PRIVATE main.cpp)
#
##add_executable(MatlangTypes main.cpp)

cmake_minimum_required(VERSION 3.16)
project(matlang)

set(CMAKE_CXX_STANDARD 20)

#include_directories(. include src types types/include types/src)

include_directories(.)
set(SOURCE_FILES
        dispatcher.cpp
        interpreter.cpp
        parser.cpp
        tokenizer.cpp
        comm.cpp
        expression.cpp
        integer.cpp
        matrix.cpp
        main.cpp
        )

#include_directories(types)

#here
#link_directories(types)
#
#add_subdirectory(types)
add_executable(${PROJECT_NAME}  ${SOURCE_FILES})
target_compile_options(${PROJECT_NAME} PRIVATE -fconcepts)

#and here
#target_link_libraries(${PROJECT_NAME} types)
