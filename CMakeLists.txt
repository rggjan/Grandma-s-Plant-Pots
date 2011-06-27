# defines GPP_SOURCE_DIR etc.
project (GPP) 
cmake_minimum_required(VERSION 2.6)

# check libs and packages (headers + lib)

# std C libraries
# find_package(stdlibs REQUIRED)

# libpthread with pthread.h
# find_package(pthread REQUIRED)

# check_include_file(argp.h HAVE_ARGP_H)

# additional compiler flags
add_definitions(-Wall -Wextra -pedantic -Werror)

# 'other' documentation files
# set(DOC_FILES AUTHORS NEWS README)
# set(DOC_PATH "share/doc/testprogram-0.1")
# install(FILES ${DOC_FILES}
#         DESTINATION ${DOC_PATH})

# Compile the sources
add_subdirectory (src)
