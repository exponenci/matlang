#!/bin/bash

source_files=()
for filename in ./src/*.cpp ./types/src/*.cpp; do
  source_files+=( "$filename" )
done

g++ -g -I ./include -I ./types/include -std=c++2a main.cpp "${source_files[@]}" -fconcepts -o matlang
