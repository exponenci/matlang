#include_directories(${CMAKE_CURRENT_LIST_DIR})
#
#add_subdirectory(include)
#add_subdirectory(src)
#
#target_include_directories(MatLang PUBLIC ${CMAKE_CURRENT_LIST_DIR})

set(types_files
        ../expression.cpp
        ../mcomplex.cpp
        ../polynomial.cpp
        ../rational.cpp
        ../expression.h
        ../integer.h
        ../matrix.h
        ../mcomplex.h
        ../object.h
        ../polynomial.h
        ../rational.h
        )
add_library(types ${types_files})