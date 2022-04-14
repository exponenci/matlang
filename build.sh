#!/bin/bash
source_files=()
for filename in ./*.cpp; do
  source_files+=($filename)
done

g++ -std=c++2a ${source_files[@]} -fconcepts -o run
