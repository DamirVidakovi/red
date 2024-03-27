cmake_minimum_required(VERSION 3.10)
project(rednet VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the packages
find_package(nlohmann_json CONFIG REQUIRED)
find_package(CURL REQUIRED)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cc")

add_library(rednet ${SOURCES})

# Link the libraries
target_link_libraries(rednet PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(rednet PRIVATE CURL::libcurl)

target_include_directories(rednet PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
