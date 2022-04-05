cmake_minimum_required(VERSION 3.16)
project(matlang)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(matlang
        types/expression.cpp
        types/expression.h
        types/integer.cpp
        types/integer.h
        types/matrix.cpp
        types/matrix.h
        types/rational.cpp
        types/rational.h
        types/mcomplex.cpp
        types/mcomplex.h
        types/polynomial.cpp
        types/polynomial.h
        interpreter.cpp
        interpreter.h
        dispatcher.cpp
        dispatcher.h
        tokenizer.h
        tokenizer.cpp
        types/object.h
        parser.cpp
        parser.h
        error.h
        main.cpp)
