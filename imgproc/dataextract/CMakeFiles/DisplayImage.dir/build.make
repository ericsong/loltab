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
CMAKE_SOURCE_DIR = /home/timothy/loltab/imgproc/main

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/timothy/loltab/imgproc/main

# Include any dependencies generated for this target.
include CMakeFiles/DisplayImage.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/DisplayImage.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/DisplayImage.dir/flags.make

CMakeFiles/DisplayImage.dir/DisplayImage.cpp.o: CMakeFiles/DisplayImage.dir/flags.make
CMakeFiles/DisplayImage.dir/DisplayImage.cpp.o: DisplayImage.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/timothy/loltab/imgproc/main/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/DisplayImage.dir/DisplayImage.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/DisplayImage.dir/DisplayImage.cpp.o -c /home/timothy/loltab/imgproc/main/DisplayImage.cpp

CMakeFiles/DisplayImage.dir/DisplayImage.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DisplayImage.dir/DisplayImage.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/timothy/loltab/imgproc/main/DisplayImage.cpp > CMakeFiles/DisplayImage.dir/DisplayImage.cpp.i

CMakeFiles/DisplayImage.dir/DisplayImage.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DisplayImage.dir/DisplayImage.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/timothy/loltab/imgproc/main/DisplayImage.cpp -o CMakeFiles/DisplayImage.dir/DisplayImage.cpp.s

CMakeFiles/DisplayImage.dir/DisplayImage.cpp.o.requires:
.PHONY : CMakeFiles/DisplayImage.dir/DisplayImage.cpp.o.requires

CMakeFiles/DisplayImage.dir/DisplayImage.cpp.o.provides: CMakeFiles/DisplayImage.dir/DisplayImage.cpp.o.requires
	$(MAKE) -f CMakeFiles/DisplayImage.dir/build.make CMakeFiles/DisplayImage.dir/DisplayImage.cpp.o.provides.build
.PHONY : CMakeFiles/DisplayImage.dir/DisplayImage.cpp.o.provides

CMakeFiles/DisplayImage.dir/DisplayImage.cpp.o.provides.build: CMakeFiles/DisplayImage.dir/DisplayImage.cpp.o

# Object files for target DisplayImage
DisplayImage_OBJECTS = \
"CMakeFiles/DisplayImage.dir/DisplayImage.cpp.o"

# External object files for target DisplayImage
DisplayImage_EXTERNAL_OBJECTS =

DisplayImage: CMakeFiles/DisplayImage.dir/DisplayImage.cpp.o
DisplayImage: CMakeFiles/DisplayImage.dir/build.make
DisplayImage: /usr/local/lib/libopencv_videostab.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_video.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_ts.a
DisplayImage: /usr/local/lib/libopencv_superres.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_stitching.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_photo.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_ocl.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_objdetect.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_nonfree.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_ml.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_legacy.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_imgproc.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_highgui.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_gpu.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_flann.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_features2d.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_core.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_contrib.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_calib3d.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_nonfree.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_ocl.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_gpu.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_photo.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_objdetect.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_legacy.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_video.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_ml.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_calib3d.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_features2d.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_highgui.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_imgproc.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_flann.so.2.4.7
DisplayImage: /usr/local/lib/libopencv_core.so.2.4.7
DisplayImage: CMakeFiles/DisplayImage.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable DisplayImage"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/DisplayImage.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/DisplayImage.dir/build: DisplayImage
.PHONY : CMakeFiles/DisplayImage.dir/build

CMakeFiles/DisplayImage.dir/requires: CMakeFiles/DisplayImage.dir/DisplayImage.cpp.o.requires
.PHONY : CMakeFiles/DisplayImage.dir/requires

CMakeFiles/DisplayImage.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/DisplayImage.dir/cmake_clean.cmake
.PHONY : CMakeFiles/DisplayImage.dir/clean

CMakeFiles/DisplayImage.dir/depend:
	cd /home/timothy/loltab/imgproc/main && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/timothy/loltab/imgproc/main /home/timothy/loltab/imgproc/main /home/timothy/loltab/imgproc/main /home/timothy/loltab/imgproc/main /home/timothy/loltab/imgproc/main/CMakeFiles/DisplayImage.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/DisplayImage.dir/depend

