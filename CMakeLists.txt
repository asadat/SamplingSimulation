cmake_minimum_required(VERSION 2.4.6 FATAL_ERROR)

SET(ProjName SamplingSimulation)


include_directories("./include")
include_directories("./TooN/include")
include_directories("/usr/include")

#link_directories(${PCL_LIBRARY_DIRS})
#link_directories("G:/Matlab/extern/lib/win32/microsoft")
link_directories("/usr/lib/x86_64-linux-gnu")

set(Samp_LIBRARIES GL GLU glut)


add_executable (${ProjName} 
            src/SamplingSim.cpp
	    src/World.cpp
	    src/FeatureTracker.cpp
            src/Visualizer.cpp

	    include/FeatureTracker.h
            include/World.h
	    include/Visualizer.h)

target_link_libraries (${ProjName} ${Samp_LIBRARIES})

# Add the required libraries for linking:
#TARGET_LINK_LIBRARIES(${ProjName}
#${MRPT_LIBS} # This is filled by FIND_PACKAGE(MRPT ...)
#"" # Optional extra libs...
#)
# End of CMakeLists.Txt file. Save it as a plain text file and the name should be exactly CMakeLists.Txt
