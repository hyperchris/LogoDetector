# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/chris/Lab/LogoDetector/fov/detect_fov

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chris/Lab/LogoDetector/fov/detect_fov

# Include any dependencies generated for this target.
include CMakeFiles/detect_fov.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/detect_fov.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/detect_fov.dir/flags.make

CMakeFiles/detect_fov.dir/detect_fov.cpp.o: CMakeFiles/detect_fov.dir/flags.make
CMakeFiles/detect_fov.dir/detect_fov.cpp.o: detect_fov.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/chris/Lab/LogoDetector/fov/detect_fov/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/detect_fov.dir/detect_fov.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/detect_fov.dir/detect_fov.cpp.o -c /home/chris/Lab/LogoDetector/fov/detect_fov/detect_fov.cpp

CMakeFiles/detect_fov.dir/detect_fov.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/detect_fov.dir/detect_fov.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/chris/Lab/LogoDetector/fov/detect_fov/detect_fov.cpp > CMakeFiles/detect_fov.dir/detect_fov.cpp.i

CMakeFiles/detect_fov.dir/detect_fov.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/detect_fov.dir/detect_fov.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/chris/Lab/LogoDetector/fov/detect_fov/detect_fov.cpp -o CMakeFiles/detect_fov.dir/detect_fov.cpp.s

CMakeFiles/detect_fov.dir/detect_fov.cpp.o.requires:
.PHONY : CMakeFiles/detect_fov.dir/detect_fov.cpp.o.requires

CMakeFiles/detect_fov.dir/detect_fov.cpp.o.provides: CMakeFiles/detect_fov.dir/detect_fov.cpp.o.requires
	$(MAKE) -f CMakeFiles/detect_fov.dir/build.make CMakeFiles/detect_fov.dir/detect_fov.cpp.o.provides.build
.PHONY : CMakeFiles/detect_fov.dir/detect_fov.cpp.o.provides

CMakeFiles/detect_fov.dir/detect_fov.cpp.o.provides.build: CMakeFiles/detect_fov.dir/detect_fov.cpp.o

# Object files for target detect_fov
detect_fov_OBJECTS = \
"CMakeFiles/detect_fov.dir/detect_fov.cpp.o"

# External object files for target detect_fov
detect_fov_EXTERNAL_OBJECTS =

detect_fov: CMakeFiles/detect_fov.dir/detect_fov.cpp.o
detect_fov: CMakeFiles/detect_fov.dir/build.make
detect_fov: /usr/local/lib/libopencv_videostab.so.2.4.10
detect_fov: /usr/local/lib/libopencv_video.so.2.4.10
detect_fov: /usr/local/lib/libopencv_ts.a
detect_fov: /usr/local/lib/libopencv_superres.so.2.4.10
detect_fov: /usr/local/lib/libopencv_stitching.so.2.4.10
detect_fov: /usr/local/lib/libopencv_photo.so.2.4.10
detect_fov: /usr/local/lib/libopencv_ocl.so.2.4.10
detect_fov: /usr/local/lib/libopencv_objdetect.so.2.4.10
detect_fov: /usr/local/lib/libopencv_nonfree.so.2.4.10
detect_fov: /usr/local/lib/libopencv_ml.so.2.4.10
detect_fov: /usr/local/lib/libopencv_legacy.so.2.4.10
detect_fov: /usr/local/lib/libopencv_imgproc.so.2.4.10
detect_fov: /usr/local/lib/libopencv_highgui.so.2.4.10
detect_fov: /usr/local/lib/libopencv_gpu.so.2.4.10
detect_fov: /usr/local/lib/libopencv_flann.so.2.4.10
detect_fov: /usr/local/lib/libopencv_features2d.so.2.4.10
detect_fov: /usr/local/lib/libopencv_core.so.2.4.10
detect_fov: /usr/local/lib/libopencv_contrib.so.2.4.10
detect_fov: /usr/local/lib/libopencv_calib3d.so.2.4.10
detect_fov: /usr/lib/x86_64-linux-gnu/libGLU.so
detect_fov: /usr/lib/x86_64-linux-gnu/libGL.so
detect_fov: /usr/lib/x86_64-linux-gnu/libSM.so
detect_fov: /usr/lib/x86_64-linux-gnu/libICE.so
detect_fov: /usr/lib/x86_64-linux-gnu/libX11.so
detect_fov: /usr/lib/x86_64-linux-gnu/libXext.so
detect_fov: /usr/local/lib/libopencv_nonfree.so.2.4.10
detect_fov: /usr/local/lib/libopencv_ocl.so.2.4.10
detect_fov: /usr/local/lib/libopencv_gpu.so.2.4.10
detect_fov: /usr/local/lib/libopencv_photo.so.2.4.10
detect_fov: /usr/local/lib/libopencv_objdetect.so.2.4.10
detect_fov: /usr/local/lib/libopencv_legacy.so.2.4.10
detect_fov: /usr/local/lib/libopencv_video.so.2.4.10
detect_fov: /usr/local/lib/libopencv_ml.so.2.4.10
detect_fov: /usr/local/lib/libopencv_calib3d.so.2.4.10
detect_fov: /usr/local/lib/libopencv_features2d.so.2.4.10
detect_fov: /usr/local/lib/libopencv_highgui.so.2.4.10
detect_fov: /usr/local/lib/libopencv_imgproc.so.2.4.10
detect_fov: /usr/local/lib/libopencv_flann.so.2.4.10
detect_fov: /usr/local/lib/libopencv_core.so.2.4.10
detect_fov: CMakeFiles/detect_fov.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable detect_fov"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/detect_fov.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/detect_fov.dir/build: detect_fov
.PHONY : CMakeFiles/detect_fov.dir/build

CMakeFiles/detect_fov.dir/requires: CMakeFiles/detect_fov.dir/detect_fov.cpp.o.requires
.PHONY : CMakeFiles/detect_fov.dir/requires

CMakeFiles/detect_fov.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/detect_fov.dir/cmake_clean.cmake
.PHONY : CMakeFiles/detect_fov.dir/clean

CMakeFiles/detect_fov.dir/depend:
	cd /home/chris/Lab/LogoDetector/fov/detect_fov && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chris/Lab/LogoDetector/fov/detect_fov /home/chris/Lab/LogoDetector/fov/detect_fov /home/chris/Lab/LogoDetector/fov/detect_fov /home/chris/Lab/LogoDetector/fov/detect_fov /home/chris/Lab/LogoDetector/fov/detect_fov/CMakeFiles/detect_fov.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/detect_fov.dir/depend
