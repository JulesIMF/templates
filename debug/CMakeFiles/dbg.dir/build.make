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
CMAKE_SOURCE_DIR = /home/julesimf/source/MIPT/Ded4/templates/containers

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/julesimf/source/MIPT/Ded4/templates/containers

# Include any dependencies generated for this target.
include debug/CMakeFiles/dbg.dir/depend.make

# Include the progress variables for this target.
include debug/CMakeFiles/dbg.dir/progress.make

# Include the compile flags for this target's objects.
include debug/CMakeFiles/dbg.dir/flags.make

debug/CMakeFiles/dbg.dir/dbg.cpp.o: debug/CMakeFiles/dbg.dir/flags.make
debug/CMakeFiles/dbg.dir/dbg.cpp.o: debug/dbg.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/julesimf/source/MIPT/Ded4/templates/containers/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object debug/CMakeFiles/dbg.dir/dbg.cpp.o"
	cd /home/julesimf/source/MIPT/Ded4/templates/containers/debug && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dbg.dir/dbg.cpp.o -c /home/julesimf/source/MIPT/Ded4/templates/containers/debug/dbg.cpp

debug/CMakeFiles/dbg.dir/dbg.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dbg.dir/dbg.cpp.i"
	cd /home/julesimf/source/MIPT/Ded4/templates/containers/debug && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/julesimf/source/MIPT/Ded4/templates/containers/debug/dbg.cpp > CMakeFiles/dbg.dir/dbg.cpp.i

debug/CMakeFiles/dbg.dir/dbg.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dbg.dir/dbg.cpp.s"
	cd /home/julesimf/source/MIPT/Ded4/templates/containers/debug && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/julesimf/source/MIPT/Ded4/templates/containers/debug/dbg.cpp -o CMakeFiles/dbg.dir/dbg.cpp.s

# Object files for target dbg
dbg_OBJECTS = \
"CMakeFiles/dbg.dir/dbg.cpp.o"

# External object files for target dbg
dbg_EXTERNAL_OBJECTS =

debug/bin/libdbg.a: debug/CMakeFiles/dbg.dir/dbg.cpp.o
debug/bin/libdbg.a: debug/CMakeFiles/dbg.dir/build.make
debug/bin/libdbg.a: debug/CMakeFiles/dbg.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/julesimf/source/MIPT/Ded4/templates/containers/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library bin/libdbg.a"
	cd /home/julesimf/source/MIPT/Ded4/templates/containers/debug && $(CMAKE_COMMAND) -P CMakeFiles/dbg.dir/cmake_clean_target.cmake
	cd /home/julesimf/source/MIPT/Ded4/templates/containers/debug && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dbg.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
debug/CMakeFiles/dbg.dir/build: debug/bin/libdbg.a

.PHONY : debug/CMakeFiles/dbg.dir/build

debug/CMakeFiles/dbg.dir/clean:
	cd /home/julesimf/source/MIPT/Ded4/templates/containers/debug && $(CMAKE_COMMAND) -P CMakeFiles/dbg.dir/cmake_clean.cmake
.PHONY : debug/CMakeFiles/dbg.dir/clean

debug/CMakeFiles/dbg.dir/depend:
	cd /home/julesimf/source/MIPT/Ded4/templates/containers && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/julesimf/source/MIPT/Ded4/templates/containers /home/julesimf/source/MIPT/Ded4/templates/containers/debug /home/julesimf/source/MIPT/Ded4/templates/containers /home/julesimf/source/MIPT/Ded4/templates/containers/debug /home/julesimf/source/MIPT/Ded4/templates/containers/debug/CMakeFiles/dbg.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : debug/CMakeFiles/dbg.dir/depend
