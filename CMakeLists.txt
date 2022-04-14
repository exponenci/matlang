cmake_minimum_required(VERSION 3.16)
project(matlang)

set(CMAKE_CXX_STANDARD 20)

include_directories(. include src types types/include types/src)

set(SOURCE_FILES
        src/comm.cpp
        src/dispatcher.cpp
        src/interpreter.cpp
        src/parser.cpp
        src/tokenizer.cpp
        types/src/expression.cpp
        types/src/integer.cpp
        types/src/matrix.cpp
        types/src/rational.cpp
        )

add_executable(${PROJECT_NAME} main.cpp ${SOURCE_FILES})
target_compile_options(${PROJECT_NAME} PRIVATE -fconcepts)

