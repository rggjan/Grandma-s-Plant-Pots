# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
 
# Create a variable called helloworld_SOURCES containing all .cpp files:
# set(GPP_SOURCES *.cpp)
 
# For a large number of source files you can create it in a simpler way
# using file() function:
file(GLOB GPP_SOURCES *.cpp plants/*.cpp bugs/*.cpp)

# Find clanlib
set(ClanLib_MAJOR_VERSION 3)
set(ClanLib_MINOR_VERSION 0)

# Add clanlib dependency
find_package(ClanLib COMPONENTS App Display GL SWRender Sound REQUIRED)

# Include clanlib
include_directories(${ClanLib_INCLUDE_DIRS} /usr/local/include/ClanLib-2.3)

# Add compiler flags
add_definitions(${ClanLib_DEFINITIONS})

# Create an executable file called helloworld from sources:
add_executable(gpp ${GPP_SOURCES})

# Link with clanlib
target_link_libraries(gpp ${ClanLib_LIBRARIES})
