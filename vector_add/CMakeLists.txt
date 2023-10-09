############################################################
#   Author - Aditya Sahu
#   https://github.com/adityasahu01/MPI_Projects
#   
#   Copyright (c), 2023
#############################################################

cmake_minimum_required(VERSION 3.12)
project(VectorAddition)

# Set C++ compiler and flags
set(CMAKE_CXX_COMPILER mpicxx)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Directories
set(SRC_DIR src)
set(INCLUDE_DIR include)
set(BIN_DIR build)

# Source files
file(GLOB SRC_FILES ${SRC_DIR}/*.cpp)

# Executable name
set(EXECUTABLE add_vector)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/build)
# set(CMAKE_LOG_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build_logs")

# Include directories
include_directories(${INCLUDE_DIR})

file(MAKE_DIRECTORY ${BIN_DIR})

# Build the main executable
add_executable(${EXECUTABLE} ${SRC_FILES})

# Specify target properties
target_include_directories(${EXECUTABLE} PRIVATE ${INCLUDE_DIR})
target_link_libraries(${EXECUTABLE} PRIVATE ${LIBS})


# Clean target: remove build artifacts
add_custom_target(clean_all
    COMMAND rm -rf ${BIN_DIR} ${EXECUTABLE} CMakeFiles CMakeCache.txt cmake_install.cmake Makefile
    COMMENT "Cleaning build artifacts"
)

# Set dependencies between the clean_all target and other targets
add_dependencies(clean_all ${EXECUTABLE})

# Add 'all' and 'clean' targets to the generated makefile
add_custom_target(build_all
    DEPENDS ${EXECUTABLE}
)
