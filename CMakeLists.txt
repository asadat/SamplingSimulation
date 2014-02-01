cmake_minimum_required(VERSION 2.4.6 FATAL_ERROR)

SET(ProjName SamplingSimulation)

#uncomment for mac
#include_directories("include")
#include_directories("/usr/local/include/TooN")
#include_directories("/usr/include")
#include_directories("/opt/X11/include")
#include_directories("/opt/local/include")
#include_directories("/usr/local/include")
#link_directories("/opt/X11/lib")
#link_directories("/usr/lib/x86_64-linux-gnu")
#link_directories("/usr/local/lib/TooN")
#link_directories("/opt/local/lib")
#link_directories("/usr/local/lib")

#uncomment for ubuntu
include_directories("~/libcvd")

include_directories("./include")
include_directories("./include/tree")
include_directories("./TooN/include")
include_directories("/usr/include")
link_directories("/usr/lib/x86_64-linux-gnu")
link_directories("/usr/lib")
link_directories("~/libcvd")

set(Samp_LIBRARIES GL GLU glut gmp CGAL CGAL_Core cvd)

SET(GCC_CGAL_COMPILE_FLAGS "-frounding-math")
#SET(GCC_CGAL_COMPILE_FLAGS "-fp-model-strict")

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_CGAL_COMPILE_FLAGS}" )

add_executable (${ProjName} 
            src/SamplingSim.cpp
	    src/World.cpp
	    src/FeatureTracker.cpp
            src/Visualizer.cpp
            src/MeshCreator.cpp
	    src/TSP.cpp
	    src/Drone.cpp
            include/Drone.h
	    include/TSP.h
            include/MeshCreator.h
	    include/FeatureTracker.h
            include/World.h
            include/Visualizer.h
            include/SamplingSim.h )

target_link_libraries (${ProjName} ${Samp_LIBRARIES})

add_executable (stats src/stats.cpp)
target_link_libraries (stats ${Samp_LIBRARIES})
