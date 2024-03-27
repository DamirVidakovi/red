cmake_minimum_required(VERSION 3.10)

# Set the project name
project(Red
  VERSION 0.1
  DESCRIPTION "Red"
  LANGUAGES CXX
)

# Set policy CMP0079 to NEW
if(POLICY CMP0079)
  cmake_policy(SET CMP0079 NEW)
endif()

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Add the reddit subdirectory
add_subdirectory(${CMAKE_SOURCE_DIR}/rednet)
add_subdirectory(${CMAKE_SOURCE_DIR}/reddcc)

message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")

add_subdirectory(${CMAKE_SOURCE_DIR}/test_bin)

target_link_libraries(test_bin PRIVATE rednet reddcc)

