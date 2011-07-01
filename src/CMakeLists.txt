# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
 
# Create a variable called helloworld_SOURCES containing all .cpp files:
# set(GPP_SOURCES *.cpp)
 
# For a large number of source files you can create it in a simpler way
# using file() function:
file(GLOB GPP_SOURCES *.cpp)

# Find clanlib
set(ClanLib_MAJOR_VERSION 2)
set(ClanLib_MINOR_VERSION 2)

# Add clanlib dependency
find_package(ClanLib COMPONENTS App Display GL GL1 Sound REQUIRED)

# Include clanlib
include_directories(${ClanLib_INCLUDE_DIRS})

# Add compiler flags
add_definitions(${ClanLib_DEFINITIONS})

# Create an executable file called helloworld from sources:
add_executable(gpp ${GPP_SOURCES})

# Link with clanlib
target_link_libraries(gpp ${ClanLib_LIBRARIES})
