# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_SOURCE_DIR = /home/paulo/frednator-2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/paulo/frednator-2

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/paulo/frednator-2/CMakeFiles /home/paulo/frednator-2/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/paulo/frednator-2/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named videoUtil

# Build rule for target.
videoUtil: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 videoUtil
.PHONY : videoUtil

# fast build rule for target.
videoUtil/fast:
	$(MAKE) -f CMakeFiles/videoUtil.dir/build.make CMakeFiles/videoUtil.dir/build
.PHONY : videoUtil/fast

src/cpp/videoUtil.o: src/cpp/videoUtil.cc.o

.PHONY : src/cpp/videoUtil.o

# target to build an object file
src/cpp/videoUtil.cc.o:
	$(MAKE) -f CMakeFiles/videoUtil.dir/build.make CMakeFiles/videoUtil.dir/src/cpp/videoUtil.cc.o
.PHONY : src/cpp/videoUtil.cc.o

src/cpp/videoUtil.i: src/cpp/videoUtil.cc.i

.PHONY : src/cpp/videoUtil.i

# target to preprocess a source file
src/cpp/videoUtil.cc.i:
	$(MAKE) -f CMakeFiles/videoUtil.dir/build.make CMakeFiles/videoUtil.dir/src/cpp/videoUtil.cc.i
.PHONY : src/cpp/videoUtil.cc.i

src/cpp/videoUtil.s: src/cpp/videoUtil.cc.s

.PHONY : src/cpp/videoUtil.s

# target to generate assembly for a file
src/cpp/videoUtil.cc.s:
	$(MAKE) -f CMakeFiles/videoUtil.dir/build.make CMakeFiles/videoUtil.dir/src/cpp/videoUtil.cc.s
.PHONY : src/cpp/videoUtil.cc.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... videoUtil"
	@echo "... edit_cache"
	@echo "... src/cpp/videoUtil.o"
	@echo "... src/cpp/videoUtil.i"
	@echo "... src/cpp/videoUtil.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

