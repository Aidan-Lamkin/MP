# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/aidan/Documents/CSCI_441/MP

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/aidan/Documents/CSCI_441/MP/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/MP.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/MP.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MP.dir/flags.make

CMakeFiles/MP.dir/main.cpp.o: CMakeFiles/MP.dir/flags.make
CMakeFiles/MP.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/aidan/Documents/CSCI_441/MP/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MP.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MP.dir/main.cpp.o -c /Users/aidan/Documents/CSCI_441/MP/main.cpp

CMakeFiles/MP.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MP.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/aidan/Documents/CSCI_441/MP/main.cpp > CMakeFiles/MP.dir/main.cpp.i

CMakeFiles/MP.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MP.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/aidan/Documents/CSCI_441/MP/main.cpp -o CMakeFiles/MP.dir/main.cpp.s

CMakeFiles/MP.dir/MPEngine.cpp.o: CMakeFiles/MP.dir/flags.make
CMakeFiles/MP.dir/MPEngine.cpp.o: ../MPEngine.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/aidan/Documents/CSCI_441/MP/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/MP.dir/MPEngine.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MP.dir/MPEngine.cpp.o -c /Users/aidan/Documents/CSCI_441/MP/MPEngine.cpp

CMakeFiles/MP.dir/MPEngine.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MP.dir/MPEngine.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/aidan/Documents/CSCI_441/MP/MPEngine.cpp > CMakeFiles/MP.dir/MPEngine.cpp.i

CMakeFiles/MP.dir/MPEngine.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MP.dir/MPEngine.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/aidan/Documents/CSCI_441/MP/MPEngine.cpp -o CMakeFiles/MP.dir/MPEngine.cpp.s

CMakeFiles/MP.dir/motorcycle.cpp.o: CMakeFiles/MP.dir/flags.make
CMakeFiles/MP.dir/motorcycle.cpp.o: ../motorcycle.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/aidan/Documents/CSCI_441/MP/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/MP.dir/motorcycle.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MP.dir/motorcycle.cpp.o -c /Users/aidan/Documents/CSCI_441/MP/motorcycle.cpp

CMakeFiles/MP.dir/motorcycle.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MP.dir/motorcycle.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/aidan/Documents/CSCI_441/MP/motorcycle.cpp > CMakeFiles/MP.dir/motorcycle.cpp.i

CMakeFiles/MP.dir/motorcycle.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MP.dir/motorcycle.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/aidan/Documents/CSCI_441/MP/motorcycle.cpp -o CMakeFiles/MP.dir/motorcycle.cpp.s

CMakeFiles/MP.dir/bobomb.cpp.o: CMakeFiles/MP.dir/flags.make
CMakeFiles/MP.dir/bobomb.cpp.o: ../bobomb.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/aidan/Documents/CSCI_441/MP/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/MP.dir/bobomb.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MP.dir/bobomb.cpp.o -c /Users/aidan/Documents/CSCI_441/MP/bobomb.cpp

CMakeFiles/MP.dir/bobomb.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MP.dir/bobomb.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/aidan/Documents/CSCI_441/MP/bobomb.cpp > CMakeFiles/MP.dir/bobomb.cpp.i

CMakeFiles/MP.dir/bobomb.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MP.dir/bobomb.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/aidan/Documents/CSCI_441/MP/bobomb.cpp -o CMakeFiles/MP.dir/bobomb.cpp.s

CMakeFiles/MP.dir/robot.cpp.o: CMakeFiles/MP.dir/flags.make
CMakeFiles/MP.dir/robot.cpp.o: ../robot.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/aidan/Documents/CSCI_441/MP/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/MP.dir/robot.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MP.dir/robot.cpp.o -c /Users/aidan/Documents/CSCI_441/MP/robot.cpp

CMakeFiles/MP.dir/robot.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MP.dir/robot.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/aidan/Documents/CSCI_441/MP/robot.cpp > CMakeFiles/MP.dir/robot.cpp.i

CMakeFiles/MP.dir/robot.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MP.dir/robot.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/aidan/Documents/CSCI_441/MP/robot.cpp -o CMakeFiles/MP.dir/robot.cpp.s

# Object files for target MP
MP_OBJECTS = \
"CMakeFiles/MP.dir/main.cpp.o" \
"CMakeFiles/MP.dir/MPEngine.cpp.o" \
"CMakeFiles/MP.dir/motorcycle.cpp.o" \
"CMakeFiles/MP.dir/bobomb.cpp.o" \
"CMakeFiles/MP.dir/robot.cpp.o"

# External object files for target MP
MP_EXTERNAL_OBJECTS =

MP: CMakeFiles/MP.dir/main.cpp.o
MP: CMakeFiles/MP.dir/MPEngine.cpp.o
MP: CMakeFiles/MP.dir/motorcycle.cpp.o
MP: CMakeFiles/MP.dir/bobomb.cpp.o
MP: CMakeFiles/MP.dir/robot.cpp.o
MP: CMakeFiles/MP.dir/build.make
MP: CMakeFiles/MP.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/aidan/Documents/CSCI_441/MP/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable MP"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MP.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MP.dir/build: MP

.PHONY : CMakeFiles/MP.dir/build

CMakeFiles/MP.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MP.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MP.dir/clean

CMakeFiles/MP.dir/depend:
	cd /Users/aidan/Documents/CSCI_441/MP/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/aidan/Documents/CSCI_441/MP /Users/aidan/Documents/CSCI_441/MP /Users/aidan/Documents/CSCI_441/MP/cmake-build-debug /Users/aidan/Documents/CSCI_441/MP/cmake-build-debug /Users/aidan/Documents/CSCI_441/MP/cmake-build-debug/CMakeFiles/MP.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MP.dir/depend

