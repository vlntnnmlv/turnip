cmake_minimum_required(VERSION 3.21)
project(turnip)

# Set global C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add bgfx
add_subdirectory(vendor/bgfx.cmake)

# Add SDL
add_subdirectory(vendor/SDL)

# Add engine
add_subdirectory(src)

# Add all examples
file(GLOB EXAMPLE_DIRS RELATIVE ${CMAKE_SOURCE_DIR}/examples ${CMAKE_SOURCE_DIR}/examples/*)
foreach(Example ${EXAMPLE_DIRS})
    if("${Example}" STREQUAL "tomove")
        continue()
    endif()

    if(IS_DIRECTORY "${CMAKE_SOURCE_DIR}/examples/${Example}")
        add_subdirectory("examples/${Example}")
    endif()
endforeach()

# Create symlink to compile_commands.json for IDE to pick it up
if (PROJECT_IS_TOP_LEVEL AND UNIX)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
    )
endif()