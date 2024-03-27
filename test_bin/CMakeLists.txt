cmake_minimum_required(VERSION 3.10)
project(test_bin VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "src/*.cc")

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# add nlohmann json
find_package(nlohmann_json REQUIRED)

# Link the library
target_link_libraries(${PROJECT_NAME} PRIVATE rednet)
target_link_libraries(${PROJECT_NAME} PRIVATE reddcc)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

# create a bin folder in root directory if it doesn't exist and create a folder for the build type
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>
        COMMENT "Creating bin folder in root directory")

if(CMAKE_BUILD_TYPE MATCHES Release)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_STRIP} $<TARGET_FILE:${PROJECT_NAME}>
        COMMENT "Stripping symbols from ${PROJECT_NAME}")
endif()

# copy the executable to the bin folder
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>
        COMMENT "Copying executable to bin folder")
