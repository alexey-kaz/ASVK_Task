# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /home/parallels/clion/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/parallels/clion/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/parallels/CLionProjects/ASVK_Extra_Task1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/parallels/CLionProjects/ASVK_Extra_Task1/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/ASVK.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ASVK.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ASVK.dir/flags.make

CMakeFiles/ASVK.dir/main.cpp.o: CMakeFiles/ASVK.dir/flags.make
CMakeFiles/ASVK.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/parallels/CLionProjects/ASVK_Extra_Task1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ASVK.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ASVK.dir/main.cpp.o -c /home/parallels/CLionProjects/ASVK_Extra_Task1/main.cpp

CMakeFiles/ASVK.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ASVK.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/parallels/CLionProjects/ASVK_Extra_Task1/main.cpp > CMakeFiles/ASVK.dir/main.cpp.i

CMakeFiles/ASVK.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ASVK.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/parallels/CLionProjects/ASVK_Extra_Task1/main.cpp -o CMakeFiles/ASVK.dir/main.cpp.s

# Object files for target ASVK
ASVK_OBJECTS = \
"CMakeFiles/ASVK.dir/main.cpp.o"

# External object files for target ASVK
ASVK_EXTERNAL_OBJECTS =

ASVK: CMakeFiles/ASVK.dir/main.cpp.o
ASVK: CMakeFiles/ASVK.dir/build.make
ASVK: CMakeFiles/ASVK.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/parallels/CLionProjects/ASVK_Extra_Task1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ASVK"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ASVK.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ASVK.dir/build: ASVK

.PHONY : CMakeFiles/ASVK.dir/build

CMakeFiles/ASVK.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ASVK.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ASVK.dir/clean

CMakeFiles/ASVK.dir/depend:
	cd /home/parallels/CLionProjects/ASVK_Extra_Task1/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/parallels/CLionProjects/ASVK_Extra_Task1 /home/parallels/CLionProjects/ASVK_Extra_Task1 /home/parallels/CLionProjects/ASVK_Extra_Task1/cmake-build-debug /home/parallels/CLionProjects/ASVK_Extra_Task1/cmake-build-debug /home/parallels/CLionProjects/ASVK_Extra_Task1/cmake-build-debug/CMakeFiles/ASVK.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ASVK.dir/depend

