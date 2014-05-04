# Project name is not mandatory, but you should use it
project(GPP)
 
# States that CMake required version must be &gt;= 2.6
cmake_minimum_required(VERSION 2.6)

# Set to release
# set(CMAKE_BUILD_TYPE Release)

## Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
endif()

if(WIN32)
#  set(CompilerFlags
#          CMAKE_CXX_FLAGS
#          CMAKE_CXX_FLAGS_DEBUG
#          CMAKE_CXX_FLAGS_RELEASE
#          CMAKE_C_FLAGS
#          CMAKE_C_FLAGS_DEBUG
#          CMAKE_C_FLAGS_RELEASE
#          )
#  foreach(CompilerFlag ${CompilerFlags})
#    string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
#  endforeach()
  set(CMAKE_CXX_FLAGS_RELEASE "/MT") ## Optimize

endif()
 
# Appends the cmake/modules path inside the MAKE_MODULE_PATH variable which stores the
# directories of additional CMake modules (eg MacroOutOfSourceBuild.cmake):
set(CMAKE_MODULE_PATH ${GPP_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})
 
# The macro below forces the build directory to be different from source directory:
include(MacroOutOfSourceBuild)
 
macro_ensure_out_of_source_build("${PROJECT_NAME} requires an out of source build.")

# Set executable path to here
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
 
add_subdirectory(src)

add_custom_target(test WORKING_DIRECTORY ${GPP_SOURCE_DIR} COMMAND python2.7 ./cpplint.py --filter=-runtime/threadsafe_fn src/*.cpp src/*.h src/*/*.cpp src/*/*.h)
add_custom_target(check WORKING_DIRECTORY ${GPP_SOURCE_DIR} COMMAND cppcheck src/*.cpp src/*.h src/*/*.cpp src/*/*.h)
add_custom_target(run WORKING_DIRECTORY ${GPP_SOURCE_DIR} COMMAND ${PROJECT_BINARY_DIR}/gpp DEPENDS gpp)
