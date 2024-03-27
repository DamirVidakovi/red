cmake_minimum_required(VERSION 3.10)
project(reddcc VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the packages
find_package(nlohmann_json CONFIG REQUIRED)
find_package(CURL REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread coroutine)  # Add the necessary components here
find_package(OpenSSL REQUIRED)  

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cc")

add_library(reddcc ${SOURCES})

# Link the libraries
target_link_libraries(reddcc PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(reddcc PRIVATE CURL::libcurl)
target_link_libraries(reddcc PRIVATE Boost::system Boost::thread Boost::coroutine)
target_link_libraries(reddcc PRIVATE OpenSSL::SSL OpenSSL::Crypto)

target_include_directories(reddcc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
