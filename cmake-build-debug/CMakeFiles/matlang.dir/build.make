# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/User/Desktop/kek/matlang

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/User/Desktop/kek/matlang/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/matlang.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/matlang.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/matlang.dir/flags.make

CMakeFiles/matlang.dir/dispatcher.cpp.o: CMakeFiles/matlang.dir/flags.make
CMakeFiles/matlang.dir/dispatcher.cpp.o: ../dispatcher.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/User/Desktop/kek/matlang/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/matlang.dir/dispatcher.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/matlang.dir/dispatcher.cpp.o -c /mnt/c/Users/User/Desktop/kek/matlang/dispatcher.cpp

CMakeFiles/matlang.dir/dispatcher.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/matlang.dir/dispatcher.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/User/Desktop/kek/matlang/dispatcher.cpp > CMakeFiles/matlang.dir/dispatcher.cpp.i

CMakeFiles/matlang.dir/dispatcher.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/matlang.dir/dispatcher.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/User/Desktop/kek/matlang/dispatcher.cpp -o CMakeFiles/matlang.dir/dispatcher.cpp.s

CMakeFiles/matlang.dir/interpreter.cpp.o: CMakeFiles/matlang.dir/flags.make
CMakeFiles/matlang.dir/interpreter.cpp.o: ../interpreter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/User/Desktop/kek/matlang/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/matlang.dir/interpreter.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/matlang.dir/interpreter.cpp.o -c /mnt/c/Users/User/Desktop/kek/matlang/interpreter.cpp

CMakeFiles/matlang.dir/interpreter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/matlang.dir/interpreter.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/User/Desktop/kek/matlang/interpreter.cpp > CMakeFiles/matlang.dir/interpreter.cpp.i

CMakeFiles/matlang.dir/interpreter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/matlang.dir/interpreter.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/User/Desktop/kek/matlang/interpreter.cpp -o CMakeFiles/matlang.dir/interpreter.cpp.s

CMakeFiles/matlang.dir/parser.cpp.o: CMakeFiles/matlang.dir/flags.make
CMakeFiles/matlang.dir/parser.cpp.o: ../parser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/User/Desktop/kek/matlang/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/matlang.dir/parser.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/matlang.dir/parser.cpp.o -c /mnt/c/Users/User/Desktop/kek/matlang/parser.cpp

CMakeFiles/matlang.dir/parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/matlang.dir/parser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/User/Desktop/kek/matlang/parser.cpp > CMakeFiles/matlang.dir/parser.cpp.i

CMakeFiles/matlang.dir/parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/matlang.dir/parser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/User/Desktop/kek/matlang/parser.cpp -o CMakeFiles/matlang.dir/parser.cpp.s

CMakeFiles/matlang.dir/tokenizer.cpp.o: CMakeFiles/matlang.dir/flags.make
CMakeFiles/matlang.dir/tokenizer.cpp.o: ../tokenizer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/User/Desktop/kek/matlang/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/matlang.dir/tokenizer.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/matlang.dir/tokenizer.cpp.o -c /mnt/c/Users/User/Desktop/kek/matlang/tokenizer.cpp

CMakeFiles/matlang.dir/tokenizer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/matlang.dir/tokenizer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/User/Desktop/kek/matlang/tokenizer.cpp > CMakeFiles/matlang.dir/tokenizer.cpp.i

CMakeFiles/matlang.dir/tokenizer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/matlang.dir/tokenizer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/User/Desktop/kek/matlang/tokenizer.cpp -o CMakeFiles/matlang.dir/tokenizer.cpp.s

CMakeFiles/matlang.dir/comm.cpp.o: CMakeFiles/matlang.dir/flags.make
CMakeFiles/matlang.dir/comm.cpp.o: ../comm.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/User/Desktop/kek/matlang/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/matlang.dir/comm.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/matlang.dir/comm.cpp.o -c /mnt/c/Users/User/Desktop/kek/matlang/comm.cpp

CMakeFiles/matlang.dir/comm.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/matlang.dir/comm.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/User/Desktop/kek/matlang/comm.cpp > CMakeFiles/matlang.dir/comm.cpp.i

CMakeFiles/matlang.dir/comm.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/matlang.dir/comm.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/User/Desktop/kek/matlang/comm.cpp -o CMakeFiles/matlang.dir/comm.cpp.s

CMakeFiles/matlang.dir/expression.cpp.o: CMakeFiles/matlang.dir/flags.make
CMakeFiles/matlang.dir/expression.cpp.o: ../expression.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/User/Desktop/kek/matlang/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/matlang.dir/expression.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/matlang.dir/expression.cpp.o -c /mnt/c/Users/User/Desktop/kek/matlang/expression.cpp

CMakeFiles/matlang.dir/expression.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/matlang.dir/expression.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/User/Desktop/kek/matlang/expression.cpp > CMakeFiles/matlang.dir/expression.cpp.i

CMakeFiles/matlang.dir/expression.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/matlang.dir/expression.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/User/Desktop/kek/matlang/expression.cpp -o CMakeFiles/matlang.dir/expression.cpp.s

CMakeFiles/matlang.dir/integer.cpp.o: CMakeFiles/matlang.dir/flags.make
CMakeFiles/matlang.dir/integer.cpp.o: ../integer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/User/Desktop/kek/matlang/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/matlang.dir/integer.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/matlang.dir/integer.cpp.o -c /mnt/c/Users/User/Desktop/kek/matlang/integer.cpp

CMakeFiles/matlang.dir/integer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/matlang.dir/integer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/User/Desktop/kek/matlang/integer.cpp > CMakeFiles/matlang.dir/integer.cpp.i

CMakeFiles/matlang.dir/integer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/matlang.dir/integer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/User/Desktop/kek/matlang/integer.cpp -o CMakeFiles/matlang.dir/integer.cpp.s

CMakeFiles/matlang.dir/matrix.cpp.o: CMakeFiles/matlang.dir/flags.make
CMakeFiles/matlang.dir/matrix.cpp.o: ../matrix.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/User/Desktop/kek/matlang/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/matlang.dir/matrix.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/matlang.dir/matrix.cpp.o -c /mnt/c/Users/User/Desktop/kek/matlang/matrix.cpp

CMakeFiles/matlang.dir/matrix.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/matlang.dir/matrix.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/User/Desktop/kek/matlang/matrix.cpp > CMakeFiles/matlang.dir/matrix.cpp.i

CMakeFiles/matlang.dir/matrix.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/matlang.dir/matrix.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/User/Desktop/kek/matlang/matrix.cpp -o CMakeFiles/matlang.dir/matrix.cpp.s

CMakeFiles/matlang.dir/main.cpp.o: CMakeFiles/matlang.dir/flags.make
CMakeFiles/matlang.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/User/Desktop/kek/matlang/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/matlang.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/matlang.dir/main.cpp.o -c /mnt/c/Users/User/Desktop/kek/matlang/main.cpp

CMakeFiles/matlang.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/matlang.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/User/Desktop/kek/matlang/main.cpp > CMakeFiles/matlang.dir/main.cpp.i

CMakeFiles/matlang.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/matlang.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/User/Desktop/kek/matlang/main.cpp -o CMakeFiles/matlang.dir/main.cpp.s

CMakeFiles/matlang.dir/rational.cpp.o: CMakeFiles/matlang.dir/flags.make
CMakeFiles/matlang.dir/rational.cpp.o: ../rational.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/User/Desktop/kek/matlang/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/matlang.dir/rational.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/matlang.dir/rational.cpp.o -c /mnt/c/Users/User/Desktop/kek/matlang/rational.cpp

CMakeFiles/matlang.dir/rational.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/matlang.dir/rational.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/User/Desktop/kek/matlang/rational.cpp > CMakeFiles/matlang.dir/rational.cpp.i

CMakeFiles/matlang.dir/rational.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/matlang.dir/rational.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/User/Desktop/kek/matlang/rational.cpp -o CMakeFiles/matlang.dir/rational.cpp.s

# Object files for target matlang
matlang_OBJECTS = \
"CMakeFiles/matlang.dir/dispatcher.cpp.o" \
"CMakeFiles/matlang.dir/interpreter.cpp.o" \
"CMakeFiles/matlang.dir/parser.cpp.o" \
"CMakeFiles/matlang.dir/tokenizer.cpp.o" \
"CMakeFiles/matlang.dir/comm.cpp.o" \
"CMakeFiles/matlang.dir/expression.cpp.o" \
"CMakeFiles/matlang.dir/integer.cpp.o" \
"CMakeFiles/matlang.dir/matrix.cpp.o" \
"CMakeFiles/matlang.dir/main.cpp.o" \
"CMakeFiles/matlang.dir/rational.cpp.o"

# External object files for target matlang
matlang_EXTERNAL_OBJECTS =

matlang: CMakeFiles/matlang.dir/dispatcher.cpp.o
matlang: CMakeFiles/matlang.dir/interpreter.cpp.o
matlang: CMakeFiles/matlang.dir/parser.cpp.o
matlang: CMakeFiles/matlang.dir/tokenizer.cpp.o
matlang: CMakeFiles/matlang.dir/comm.cpp.o
matlang: CMakeFiles/matlang.dir/expression.cpp.o
matlang: CMakeFiles/matlang.dir/integer.cpp.o
matlang: CMakeFiles/matlang.dir/matrix.cpp.o
matlang: CMakeFiles/matlang.dir/main.cpp.o
matlang: CMakeFiles/matlang.dir/rational.cpp.o
matlang: CMakeFiles/matlang.dir/build.make
matlang: CMakeFiles/matlang.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/User/Desktop/kek/matlang/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX executable matlang"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/matlang.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/matlang.dir/build: matlang

.PHONY : CMakeFiles/matlang.dir/build

CMakeFiles/matlang.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/matlang.dir/cmake_clean.cmake
.PHONY : CMakeFiles/matlang.dir/clean

CMakeFiles/matlang.dir/depend:
	cd /mnt/c/Users/User/Desktop/kek/matlang/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/User/Desktop/kek/matlang /mnt/c/Users/User/Desktop/kek/matlang /mnt/c/Users/User/Desktop/kek/matlang/cmake-build-debug /mnt/c/Users/User/Desktop/kek/matlang/cmake-build-debug /mnt/c/Users/User/Desktop/kek/matlang/cmake-build-debug/CMakeFiles/matlang.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/matlang.dir/depend

