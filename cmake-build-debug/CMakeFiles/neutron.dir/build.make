# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /home/mintgreen/clion-2019.1/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/mintgreen/clion-2019.1/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mintgreen/CLionProjects/neutron

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mintgreen/CLionProjects/neutron/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/neutron.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/neutron.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/neutron.dir/flags.make

CMakeFiles/neutron.dir/neutron/main.cpp.o: CMakeFiles/neutron.dir/flags.make
CMakeFiles/neutron.dir/neutron/main.cpp.o: ../neutron/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mintgreen/CLionProjects/neutron/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/neutron.dir/neutron/main.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/neutron.dir/neutron/main.cpp.o -c /home/mintgreen/CLionProjects/neutron/neutron/main.cpp

CMakeFiles/neutron.dir/neutron/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/neutron.dir/neutron/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mintgreen/CLionProjects/neutron/neutron/main.cpp > CMakeFiles/neutron.dir/neutron/main.cpp.i

CMakeFiles/neutron.dir/neutron/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/neutron.dir/neutron/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mintgreen/CLionProjects/neutron/neutron/main.cpp -o CMakeFiles/neutron.dir/neutron/main.cpp.s

# Object files for target neutron
neutron_OBJECTS = \
"CMakeFiles/neutron.dir/neutron/main.cpp.o"

# External object files for target neutron
neutron_EXTERNAL_OBJECTS =

neutron: CMakeFiles/neutron.dir/neutron/main.cpp.o
neutron: CMakeFiles/neutron.dir/build.make
neutron: CMakeFiles/neutron.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mintgreen/CLionProjects/neutron/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable neutron"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/neutron.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/neutron.dir/build: neutron

.PHONY : CMakeFiles/neutron.dir/build

CMakeFiles/neutron.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/neutron.dir/cmake_clean.cmake
.PHONY : CMakeFiles/neutron.dir/clean

CMakeFiles/neutron.dir/depend:
	cd /home/mintgreen/CLionProjects/neutron/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mintgreen/CLionProjects/neutron /home/mintgreen/CLionProjects/neutron /home/mintgreen/CLionProjects/neutron/cmake-build-debug /home/mintgreen/CLionProjects/neutron/cmake-build-debug /home/mintgreen/CLionProjects/neutron/cmake-build-debug/CMakeFiles/neutron.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/neutron.dir/depend
